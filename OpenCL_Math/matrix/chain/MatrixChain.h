/*************************************************************************
 *
 * MatrixChain.h
 *
 * Created by dmitry
 * 22.02.2021
 *
 ***/

#pragma once

//std
#include <vector>
#include <stack>
#include <optional>
#include <algorithm>

//my lib
#include <matrix/Matrix.h>
#include <binary_tree/binary_tree.h>


namespace ezg
{

    /// Class that implements multiplication of a chain of matrices in different ways.
    ///
    /// \invariant
    /// The matrices in the chain have a valid size.
    /// If the setOptimalOrder method was called (without a subsequent
    /// call to setDefaultOrder), then the current order is optimal.
    ///
    /// \tparam T
    ///     type of matrix data
    template< typename T >
    class MatrixChain
    {
    public:

        using matrix_type = matrix::Matrix< T >;

    private:

        /// used in the implementation of the algorithm.
        template< typename Val_ >
        using tree = ezg::binary_tree_t< Val_ >;
        /// used in the implementation of the algorithm.
        using matrix_tree = tree< matrix_type >;

    private:

        /// a chain of matrices. grows from left to right.
        std::vector< matrix_type > m_chain;
        /// the tree of the order of multiplication of matrices. (see MatrixChain.inl)
        tree< size_t >             m_curOrderTree;
        /// order of matrix interleaving. Each element of this array is an operation number.
        /// For example, multiplying matrices with indices 5 and 6 is operation number 5.
        /// (perhaps in the future will completely replace the tree).
        std::vector< size_t >      m_curOrderArr;
        /// the current order is optimal or not.
        bool                       m_isOptimal = false;

    public:

        /// Add a matrix to the end of the chain.
        /// \return matrix id in vector(todo)
        size_t addMatrix(const matrix_type& matrix);

        /// Perform multiplication with the current order.
        /// \Note
        /// To multiply in an optimum manner, before calling this function call setOptimalOrder().
        /// \return result matrix
        matrix_type multiplication() const;

        /// Sets the current in the optimal order.
        /// \Note
        /// Subsequent calls to the addMatrix function will cause this function to be called automatically.
        /// You can cancel this by calling the setDefaultOrder method.
        /// \return void
        void setOptimalOrder();

        std::vector< size_t > getCurrentOrder() const;

        /// Sets the order to normal(default).
        /// And cancels the automatic calculation of the optimal order when calling addMatrix().
        /// \return void
        void setDefaultOrder();

        /// Get the number of operations in accordance with the established procedure.
        /// \return number of operations
        size_t getNumReqOperations() const;

    private:


        void buildOptimalOrder_();

        void buildDefaultOrder_();

        matrix_tree buildMatrixTree_(const tree< size_t >& index_tree) const;

        matrix_type multiplicationByTree_(matrix_tree mTree) const;
    };//class MatrixChain


}//namespace ezg


#include "MatrixChain.inl"