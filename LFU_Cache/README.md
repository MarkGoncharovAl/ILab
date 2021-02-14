# LFU - cashe algorithm #

	Now the construction is std::multimap with frequency and id. Hash_map helps us to find element with exact id


##### Input_Tests - full declaration of tests #####
*	First param:  cashe's size
*	Second param: count id
*	Next: id

##### Output_Tests #####
*	Every string - current cache. 
*	In the item: first - id, second - frequency

##### InputRandomTests #####
*	First param:  cashe's size
*	Second param: count id
*	Third param: range of int's

##### OuputRandomTests and OutputRandomTests_map #####
It's obviously... The only difference: _map is using std::multimap O(log_2[N]) and without _map is using std::list and algorithm O(n) to find min element
