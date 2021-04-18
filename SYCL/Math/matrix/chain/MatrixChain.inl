/****************************************************************************************
 *
 *   MatrixChain.inl
 *
 *   Created by dmitry
 *   22.02.2021
 *
 ***/


//
/// MatrixChain
///======================================================================================
///
/// In this solution, we will consider approaches to multiplying a chain of matrices.
/// As a rule, the multiplication of matrices of order is not optimal. Let's say you
/// have a 10x30 A, 30 x 5 B, and 5x60 C matrix.
///
/// Calculating their product as A (BC) would take(30 × 5 × 60) + (10 × 30 × 60) = 27000
/// multiplications. At the same time, their product as (A B) C will give the same result
/// in 4500 operations.
///
/// As a solution, it is proposed to reduce the task to subtasks. Denote the result of
/// multiplying the matrices A (i) * A(i+1) * ... * A(j), where i <= j. If i < j, then
/// there is a 'k' that splits this chain between the matrices A(k) and A(k+1), i <= k < j.
/// That is, to calculate A (i,..., j), first calculate A(i,..., k),
/// then A(k+1,..., j) and then multiply them.
/// Going through all 'k', we select such that the number of required operations is minimal.
///
/// Choosing 'k' is equivalent to placing parentheses.
/// For example, for some matrices, the following multiplication order is optimal:
/// ((A * B) * (C * D))
/// It is convenient to represent such a record as a binary tree.
///
/// Two types of trees can be found in algorithms:
/// 1. Index tree. A tree whose leaves contain the numbers of matrices (equal to the ID in
/// the array of matrices), and in the nodes the number of operations required for
/// multiplying children. It is constructed using methods buildOptimalOrderTree_
/// and buildDefaultOrderTree_.
/// 2. Matrix tree. it looks the same as the index tree, but the leaves contain the
/// matrices corresponding to the index in the index tree.
/// The space in the nodes is reserved for the result of multiplying children.
/// It is constructed using methods buildMatrixTree_.
///======================================================================================
///======================================================================================

#pragma once


#include "MatrixChain.h"

namespace ezg
{

    template< typename T >
    size_t MatrixChain< T >::addMatrix(const matrix_type& matrix)
    {
        if (!m_chain.empty() && matrix.getLines() != m_chain.back().getColumns()) {
            throw std::invalid_argument("invalid matrix size");
        }
        const size_t idMatrix = m_chain.size();

        //If the order was optimal, then recalculate the new one.
        if(m_isOptimal) {
            m_chain.push_back(matrix);
            m_isOptimal = false;
            setOptimalOrder();
        }
        else {
            ///==---------------------begin_tree_code-----------------------------==
            if (m_chain.empty())
            {//the tree contains one node
                const auto idRoot = m_curOrderTree.createNode(idMatrix);
                m_curOrderTree.setRootId(idRoot);
            }
            else
            {//attach a new node with the number of the new matrix on the right.
                /*
                 *              newIdRoot
                 *             /        \
                 *         oldTree    idMatrixNode
                 */
                const size_t actions = m_curOrderTree.at(m_curOrderTree.getRootId())
                        + m_chain[0].getLines() * matrix.getLines() * matrix.getColumns();
                const auto newIdRoot = m_curOrderTree.createNode(actions);
                const auto IdMatrixNode = m_curOrderTree.createNode(idMatrix);

                m_curOrderTree.setLeft(newIdRoot, m_curOrderTree.getRootId());
                m_curOrderTree.setRight(newIdRoot, IdMatrixNode);

                m_curOrderTree.setRootId(newIdRoot);
            }
            ///==----------------------end_tree_code------------------------------==

            ///==---------------------begin_array_code----------------------------==
            if (!m_chain.empty()) {
                m_curOrderArr.push_back(m_chain.size() - 1);
            }
            ///==----------------------end_array_code-----------------------------==

            m_chain.push_back(matrix);
        }

        return idMatrix;
    }


    template< typename T >
    void MatrixChain< T >::setOptimalOrder()
    {
        if (!m_isOptimal)
        {
            buildOptimalOrder_();
            m_isOptimal = true;
        }
    }


    template< typename T >
    std::vector< size_t > MatrixChain< T >::getCurrentOrder() const
    {
        return m_curOrderArr;
    }


    template< typename T >
    void MatrixChain< T >::setDefaultOrder()
    {
        if (m_isOptimal)
        {
            buildDefaultOrder_();
            m_isOptimal = false;
        }
    }


    template< typename T >
    size_t MatrixChain< T >::getNumReqOperations() const
    {
        if (m_chain.size() <= 1u) {
            return 0;
        }

        return m_curOrderTree.at(m_curOrderTree.getRootId());
    }


    template< typename T >
    typename MatrixChain< T >::matrix_type MatrixChain< T >::multiplication() const
    {
        return multiplicationByTree_( buildMatrixTree_( m_curOrderTree ) );
    }


    //private:


    template< typename T >
    void MatrixChain< T >::buildOptimalOrder_()
    {
        /*
         * at the moment, the function also builds the order of operations in the
         * form of an array. In the future, the tree can be completely removed
         * from the implementation!
         *
         * The tree building code is marked as begin/end_tree_code.
         * The arr building code is marked as begin/end_array_code.
         */

        ///==---------------------begin_tree_code-----------------------------==
        tree< size_t > res_tree;
        //the matrix contains the indices of the nodes in the tree.
        //the node with coordinates i, j contains the optimal order
        //of multiplication of matrices Аi .... Aj.
        matrix::Matrix< size_t > indexTrees(m_chain.size(), m_chain.size());
        for (size_t l = 0, ml = indexTrees.getLines(); l < ml; l++) {
            for (size_t c = l, mc = indexTrees.getColumns(); c < mc; c++) {
                indexTrees.at(l, c) = res_tree.createNode(std::numeric_limits< size_t >::max());
            }
        }
        ///==----------------------end_tree_code------------------------------==


        ///==---------------------begin_array_code----------------------------==
        // This matrix stores partitions at which multiplication is optimal.
        // For example, if for the interval from i to j the most advantageous
        // partition is k (i <= k < j), then
        // partitionMatrix[i][j] = k;
        //
        // This allows you to restore the full order of operations, which is done
        // at the end of this function.
        matrix::Matrix< size_t > partitionMatrix(m_chain.size(), m_chain.size());
        ///==----------------------end_array_code-----------------------------==


        //stores the results of calculations to avoid repeated.
        //O(n!) -> O(n^3)
        matrix::Matrix< std::optional< size_t > > minActions(m_chain.size(), m_chain.size());

        //recursion stack.
        //  first - i
        //  second - j
        //      next step find minAction[i][j].
        std::stack< std::pair< size_t, size_t > > recursionStack;
        if (!m_chain.empty()) {
            recursionStack.push({ 0, m_chain.size() - 1 });
        }

        while(!recursionStack.empty())
        {
            const size_t oldStackSize = recursionStack.size();

            const std::pair< size_t, size_t > curPair = recursionStack.top();
            const size_t left = curPair.first;
            const size_t right = curPair.second;

            if (minActions.at(left, right).has_value())
            {//the value has already been calculated.
                recursionStack.pop();
                continue;
            }

            if (left == right)
            {//the range of length zero is the matrix. zero operations required.
                minActions.at(left, left) = 0;

                ///==---------------------begin_tree_code-----------------------------==
                res_tree.at(indexTrees.at(left, left)) = left;
                ///==----------------------end_tree_code------------------------------==

                recursionStack.pop();
                continue;
            }

            for (size_t k = left; k < right; k++)
            {//add to the stack all available permutations in the current range.
                if(!minActions.at(left, k).has_value()) {
                    recursionStack.push({ left, k });
                }
                if(!minActions.at(k + 1, right).has_value()) {
                    recursionStack.push({ k + 1, right });
                }
            }

            if (recursionStack.size() == oldStackSize)
            {//all elements are ready
                size_t sought_k = 0;
                size_t resActions = std::numeric_limits< size_t >::max();
                const size_t leftSize = m_chain.at(left).getLines();
                const size_t rightSize = m_chain.at(right).getColumns();

                for (size_t k = left; k < right; k++)
                {
                    const size_t curActions = minActions.at(left, k).value()
                                              + minActions.at(k + 1, right).value()
                                              + leftSize * m_chain.at(k).getColumns() * rightSize;

                    if (curActions < resActions) {
                        resActions = curActions;
                        sought_k = k;
                    }
                }

                ///==---------------------begin_tree_code-----------------------------==
                const size_t idLR = indexTrees.at(left, right);
                assert(resActions != std::numeric_limits< size_t >::max());
                assert(!res_tree.getLeftOptional(idLR).has_value());
                assert(!res_tree.getRightOptional(idLR).has_value());

                res_tree.at(idLR) = resActions;

                res_tree.setLeft(idLR, indexTrees.at(left, sought_k));
                res_tree.setRight(idLR, indexTrees.at(sought_k + 1, right));
                ///==----------------------end_tree_code------------------------------==


                ///==---------------------begin_array_code----------------------------==
                assert(resActions != std::numeric_limits< size_t >::max());

                partitionMatrix.at(left, right) = sought_k;
                ///==----------------------end_array_code-----------------------------==


                minActions.at(left, right) = resActions;
            }
        }

        //std::cout << partitionMatrix << std::endl;

        if (!m_chain.empty()) {
            assert(minActions.at(0, m_chain.size() - 1).has_value());

            ///==---------------------begin_tree_code-----------------------------==
            res_tree.setRootId(indexTrees.at(0, m_chain.size() - 1));
            m_curOrderTree = res_tree;
            ///==----------------------end_tree_code------------------------------==


            ///==---------------------begin_array_code----------------------------==
            // This code restores the order of operations on the partition matrix.
            // The algorithm is as follows:
            // 1. put a pair of 0, n - 1 in the deque. In these coordinates lies the
            //    optimal partition for the entire range
            //      (respectively, the last operation).
            // 2. in the loop, until the deck is empty, we perform the following:
            //  2.1. we take out a pair from the front of the deque.
            //  2.2. if the elements of the pair are the same, then we move on to the
            //       next iteration(a range of one element does not require recording any
            //       operation, there is already one matrix).
            //  2.3  we write the partition number in the 'cur_id' cell of the array;
            //       cur_id--; (this way we fill in the array of the order of operations
            //       from the end).
            //  2.4  we put two ranges at the end of the deque, into which the current
            m_curOrderArr.resize(m_chain.size() - 1);

            std::deque< std::pair< size_t, size_t > > deque;
            deque.push_back({ 0, m_chain.size() - 1 });

            size_t cur_id = m_chain.size() - 2;
            while (!deque.empty())
            {
                auto cur = deque.front();
                deque.pop_front();
                if (cur.first == cur.second) {
                    continue;
                }

                size_t idOperation = partitionMatrix.at(cur.first, cur.second);

                m_curOrderArr.at(cur_id) = idOperation;
                cur_id--;

                deque.emplace_back( cur.first, idOperation );
                deque.emplace_back( idOperation + 1, cur.second);
            }
            ///==----------------------end_array_code-----------------------------==
        }
        else
        {
            m_curOrderTree = tree< size_t >();
            m_curOrderArr  = std::vector< size_t >();
        }
    }


    template< typename T >
    void MatrixChain< T >::buildDefaultOrder_()
    {
        ///==---------------------begin_tree_code-----------------------------==
        tree< size_t > result;
        if (m_chain.size() == 0) { m_curOrderTree = result; return; }

        result.createNode(0);
        if (m_chain.size() == 1) { m_curOrderTree = result; return; }


        result.setLeft(0, result.createNode(0));
        result.setRight(0, result.createNode(1));
        result.at(0) = m_chain[0].getLines() * m_chain[0].getColumns() * m_chain[1].getColumns();

        for(size_t k = 2, mk = m_chain.size(); k < mk; k++)
        {
            const size_t actions = result.at(result.getRootId()) + m_chain[0].getLines() * m_chain[k].getColumns() * m_chain[k].getLines();

            const size_t idNewNode = result.createNode(actions);
            result.setLeft(idNewNode, result.getRootId());
            result.setRight(idNewNode, result.createNode(k));

            result.setRootId(idNewNode);
        }

        m_curOrderTree = result;
        ///==----------------------end_tree_code------------------------------==


        ///==---------------------begin_array_code----------------------------==
        if (m_chain.size() > 1) {
            m_curOrderArr.resize(m_chain.size());
            for (size_t i = 0, mi = m_chain.size() - 1; i < mi; i++) {
                m_curOrderArr[i] = i;
            }
        }
        ///==----------------------end_array_code-----------------------------==
    }


    template< typename T >
    typename MatrixChain< T >::matrix_type MatrixChain< T >::multiplicationByTree_(matrix_tree tree) const
    {
        if (m_chain.empty()) {
            return {};
        }

        if (m_chain.size() == 1) {
            return m_chain[0];
        }

        std::stack< size_t > recursionStack;
        recursionStack.push(tree.getRootId());

        while(!recursionStack.empty())
        {
            const size_t curNode = recursionStack.top();
            matrix_type& curMatrix = tree.at(curNode);

            //assert(curNode->size() == 2 || curNode->empty());

            if (curMatrix.empty() && tree.haveChildren(curNode))
            {
                const size_t left = tree.getLeftOptional(curNode).value();
                const size_t right = tree.getRightOptional(curNode).value();

                if (tree.at(left).empty()) {
                    recursionStack.push(left);
                    continue;
                }
                if (tree.at(right).empty()) {
                    recursionStack.push(right);
                    continue;
                }

                curMatrix = std::move(tree.at(left));
                curMatrix.multiplication(tree.at(right));

                recursionStack.pop();
            }

        }

        return tree.at(tree.getRootId());
    }


    template< typename T >
    typename MatrixChain< T >::matrix_tree MatrixChain< T >::buildMatrixTree_(const tree< size_t >& index_tree) const
    {
        if (m_chain.empty()) { return {}; }

        matrix_tree result;
        result.resize(index_tree.size());

        const size_t nNodes = index_tree.size();

        for (size_t k = 0; k < nNodes; k++)
        {
            if(!index_tree.haveChildren(k)) {
                result.at(k) = m_chain[index_tree.at(k)];
            }
            else
            {
                result.setLeft(k, index_tree.getLeftOptional(k).value());
                result.setRight(k, index_tree.getRightOptional(k).value());
            }
        }
        result.setRootId(index_tree.getRootId());

        return result;
    }
}//namespace ezg