#include <iostream>

using namespace std;

template <typename T>
class SortedList{
	struct Node{
		T data_;
		Node* next_;
		Node* prev_;
		Node(const T& data=T{},Node* nx=nullptr,Node* pr=nullptr){
			data_ = data;
			next_ = nx;
			prev_ = pr;
		}
	};
	Node* front_;
	Node* back_;
	int count_;

public:
	class const_iterator{
		friend class SortedList;
	protected:
		Node* curr_;
		const_iterator(Node* n) {	curr_ = n;}
	public:
		const_iterator(){
			curr_ = nullptr;
		}
		//Const iterator for the i++ operator will return this node
		const_iterator operator++() const{
			curr_ = curr_ -> next;
			return *this;
		}
		//Const iterator for the ++i operator will return previous node after incrementation
		const_iterator operator++(int){
			const_iterator old = *this;
			curr_ = curr_ -> next_;
			return old;
		}
		//Const iterator for the i-- operator will return this node
		const_iterator operator--() const{
			curr_ = curr_ -> prev_;
			return *this;
		}
		//Const iterator for the --i operator will return previous node after decrementation
		const_iterator operator--(int){
			const_iterator old = *this;
			curr_ = curr_ -> prev_;
			return old;
		}
		//Comparison if nodes are equal will return bool
		bool operator==(const_iterator rhs){
			return curr_ == rhs.curr_;
		}
		//Comparison if nodes do not equal will return bool
		bool operator!=(const_iterator rhs){
			return curr_ != rhs.curr_;
		}
		//Calls the data within the node of the iterator and returns it
		const T& operator*()const{
			return curr_ -> data_;
		}
		//Calls the node within the iterator and returns it
		Node* currNode(){
			return curr_;
		}
	};
	class iterator:public const_iterator{
		friend class const_iterator;
	public:
		//Sets the node of the iterator to nullptr
		iterator(){
			this -> curr_ = nullptr;
		}
		//Sets the node within the iterator to n
		iterator(Node* n) : const_iterator(n){}
		//Const iterator for the i++ operator will return this node
		iterator operator++(){
			this -> curr_ = this -> curr_ -> next_;
			return *this;
		}
		//Const iterator for the ++i operator will return previous node after incrementation
		iterator operator++(int a){
			iterator old = *this;
			this -> curr_ =  this -> curr_ -> next_;
			return old;
		}
		//Const iterator for the i-- operator will return this node
		iterator operator--(){
			this -> curr_ = this -> curr_ -> prev_;
			return *this;
		}
		//Const iterator for the --i operator will return previous node after decrementation
		iterator operator--(int a){
			iterator old = *this;
			this -> curr_ = this -> curr_ -> prev_;
			return old;
		}
		//Operator == will confirm that the node within rhs is equal to the node within the current iterator and return a bool
		bool operator==(iterator rhs){
			return ((this -> curr_ == nullptr && rhs.currNode() == nullptr ) ? true :
							this -> curr_ == rhs.currNode());
		}
		//Operator != will confirm that the node within rhs is not equal to the node within the current iterator and return a bool
		bool operator!=(iterator rhs){
			return this -> curr_ != rhs.curr_;
		}

		//Will return the data of the node within the iterator
		T& operator*(){
			return const_iterator::currNode() -> data_;
		}
		//Will return the const data of the node within the iterator
		const T& operator*()const{
			return const_iterator::operator*();
		}
	};

	SortedList();
	iterator begin();
	iterator end();
	const_iterator begin() const;
	const_iterator end() const;
	iterator insert(const T& data);
	iterator search(const T& data);
	const_iterator search(const T& data) const;
	iterator erase(iterator it);
	iterator erase(iterator first, iterator last);
	bool empty() const;
	int size() const;
	~SortedList();
	SortedList(const SortedList& rhs);
	SortedList& operator=(const SortedList& rhs);
	SortedList(SortedList&& rhs);
	SortedList&& operator=(SortedList&& rhs);
};

/*Default constructor for the SortedList. Sets front and back as new nodes that are connected
* to one another as well as sets count_ to 0 to initialize it.
*/
template <typename T>
SortedList<T>::SortedList(){
	front_ = new Node();
	back_ = new Node();
	front_ -> next_ = back_;
	back_ -> prev_ = front_;
	count_ = 0;
}

//The begin function is meant to return an iterator to the node of the front of the list
template <typename T>
typename SortedList<T>::iterator SortedList<T>::begin(){
	return iterator(front_ == nullptr ? nullptr : front_ -> next_);
}
//The begin function is meant to return a const iterator to the node with data in the front of the list
template <typename T>
typename SortedList<T>::const_iterator SortedList<T>::begin()const{
	return const_iterator(front_ == nullptr ? nullptr : front_ -> next_);
}
//The end function is meant to return an iterator to the node of the back of the list (back_)
template <typename T>
typename SortedList<T>::iterator SortedList<T>::end(){
	return iterator(back_);
}
//The end function is meant to return a const iterator to the node of the back of the list (back_)
template <typename T>
typename SortedList<T>::const_iterator SortedList<T>::end()const{
	return const_iterator(back_);
}

/**
* The insert member function takes in a constant data value of type T
* and returns an iterator to the newly created node. This member function
* also sorts the list from smallest to greatest by first comparing the
* incoming data with the data in each node of the list. If the incoming data
* is smaller then the node data that it is being compared to, a new node would be
* created with the incoming data and be placed to the right of the found node.
* connecting them together whilst keeping the connections of all other nodes within
* the list. If there are no nodes with data or the incoming data is larger then
* all of the data within the list, the code will create a new node with the incoming
* data and insert it within the front of the list, since it will be traversing the
* list from the back to the front.
*/
template <typename T>
typename SortedList<T>::iterator SortedList<T>::insert(const T& data){
	//nodeIn is the node that will be inserted into the list
	Node* nodeIn;
	//smallNode is a node of the smallest node within the list from the back to the front
	Node* smallNode;
	/*tempFront is used as a placement holder for the front node of the new node.
	* The information of the front node is taken from the node that is being
	* compared to within the for loop below.
	*/
	Node* tempFront;
	/*beg is an iterator to the front sentinel node while i is an iterator to the
	* last node in the list with data.
	*/
	iterator beg = iterator(front_);
	iterator i = iterator(back_ -> prev_);
	/*This loops through the nodes within the list. If there are no nodes
	* within the list, the for loop is skipped. Otherwise, if the incoming
	* data has been found to be smaller then the data within the compared
	* node, a break statement is called so that operations can occur on the
	* smallNode that has been found by the iterator i.
	*/
	for(;i != beg;i--){
		if(*i < data)
			break;
	}
	// smallNode is being set to the node of the iterator i.
	smallNode = i.currNode();
	// This is the insertion by connection and creation of the new node nodeIn.
	tempFront = smallNode -> next_;
	nodeIn = new Node(data,tempFront,smallNode);
	tempFront -> prev_ = nodeIn;
	smallNode -> next_ = nodeIn;
	count_++;

	//Returns an iterator to nodeIn;
	return iterator(nodeIn);
}
/*The search function traverses the list, comparing the incoming data with the
* data within each node of the list. This funciton will return an iterator to the
* found node, unless the node is not found in which case the end() funciton will be called.
*/
template <typename T>
typename SortedList<T>::iterator SortedList<T>::search(const T& data){
	for(auto i = begin(); i != end(); i++){
		if(*i == data)
			return i;
	}
	return end();
}
/*The search function traverses the list, comparing the incoming data with the
* data within each node of the list. This funciton will return a const iterator to the
* found node, unless the node is not found in which case the end() funciton will be called.
*/
template <typename T>
typename SortedList<T>::const_iterator SortedList<T>::search(const T& data) const{
	for(auto i = begin(); i != end(); i++){
		if(*i == data)
			return i;
	}
	return end();
}

/*This function will erase the node that is within the iteraor by taking its next_
* and prev_ connections and connecting them with one another, then the deletion of
* the node occurs by calling delete. This function will return an iterator the
* next node within the list after the node that has been removed.
*/
template <typename T>
typename SortedList<T>::iterator SortedList<T>::erase(iterator it){
	Node* currNode = it.currNode();
	Node* tempPrev = currNode -> prev_;
	Node* tempNext = currNode -> next_;
  tempPrev -> next_ = tempNext;
  tempNext -> prev_ = tempPrev;
	delete currNode;
	count_--;
	return iterator(tempNext);
}
/*This function will erase the node that is within the iteraor by taking its next_
* and prev_ connections and connecting them with one another, then the deletion of
* the node occurs by calling delete. This function will return a const iterator the
* next node within the list after the node that has been removed.
*/
template <typename T>
typename SortedList<T>::iterator SortedList<T>::erase(iterator first, iterator last){
	for(auto i = first; i != last; i++)
		erase(i);
	return last;
}
//This function will return true if the list is empty and false if it isn't
template <typename T>
bool SortedList<T>::empty() const{
	return front_ == nullptr || size() == 0;
}
//This function will return an int of the size of the list
template <typename T>
int SortedList<T>::size() const{
	return count_;
}
/*This function is the destructor of the SortedList. This destructor would loop through
* the list and call erase on each node. Once every node has been erased, the front_ and back_
* are set to nullptr and count_ is set to 0.
*/
template <typename T>
SortedList<T>::~SortedList(){
	if(!empty()){
		for(auto i = begin(); i != end(); i++)
			erase(i);
	}
	front_ = back_ = nullptr;
	count_ = 0;
}
/*This function is the copy constructor for the SortedList. It deep copies the content within
* rhs list into the currently selected list. Since it is still a constructor, the front_ and back_
* are intitialized as new sentinel nodes and attached to one another. Then the insert function is
* called on the nodes of rhs, which would insert each rhs node into the currently selected list.
*/
template <typename T>
SortedList<T>::SortedList(const SortedList& rhs){
	if(!rhs.empty()){
		front_ = new Node();
		back_ = new Node();
		front_ -> next_ = back_;
		back_ -> prev_ = front_;
		count_ = 0;

		for(auto i = rhs.begin(); i != rhs.end(); i++)
			insert(*i);
	}
	else
		front_ = back_ = nullptr;
}
/*This function is the copy operator. If this list is a nulled list, the code Will
* initialize the sentinel nodes and connect them with one another. Otherwise if the list
* is not empty, the code will erase all nodes within the current list to clean the memory,
* then the code will loop through the rhs list in order to insert each node into the
* currently selected list. This will return the currently selected list.
*/
template <typename T>
SortedList<T>& SortedList<T>::operator=(const SortedList& rhs){
	if(this != &rhs && !rhs.empty()){
		if(front_ == nullptr){
			front_ = new Node();
			back_ = new Node();
			front_ -> next_ = back_;
			back_ -> prev_ = front_;
			count_ = 0;
		}
		else{
			for(auto i = begin();i != end(); i++)
				erase(i);
		}
		for(auto i = rhs.begin(); i != rhs.end(); i++)
			insert(*i);
	}
	else
		front_ = back_ = nullptr;

	return *this;
}
/*This function is the move contructor. the purpose of the move constructor
* is to shallow copy the content of rhs into the current list. This occurs
* by setting the currently selected list's front_ and back_ sentinel nodes to
* point to the connection of the front_ and back_ of the rhs list. Afterwards,
* the front_ and back_ of rhs will be set to nullptr to disconnect those sentinel
* nodes from their data nodes.
*/
template <typename T>
SortedList<T>::SortedList(SortedList&& rhs){
	if(!rhs.empty()){
		front_ = rhs.front_;
		back_ = rhs.back_;
		count_ = rhs.size();
		rhs.front_ = nullptr;
		rhs.back_ = nullptr;
	}
	else
		front_ = back_ = nullptr;
}
/*This function is the move operator. The move operator will first confirm that
* rhs list is not empty and that this list does not equal to rhs. Once that is
* confirmed, the move operation will commence. Since the destructor will be called
* on rhs when it is out of scope, a simple swap of connections of the front_ and back_
* sentinel nodes between the current list and rhs will end up destroying the unused
* nodes within the currently selected list whilst moving the nodes within rhs to
* the current list. Finally the size of this list is set to the count_ witin rhs. Will
* return the currently selected list.
*/
template <typename T>
SortedList<T>&& SortedList<T>::operator=(SortedList&& rhs){
	if(this != &rhs && !rhs.empty()){
		Node* tempNode;
		tempNode = front_;
		front_ = rhs.front_;
		rhs.front_ = tempNode;
		tempNode = back_;
		back_ = rhs.back_;
		rhs.back_ = tempNode;
		count_ = rhs.size();
	}
	else
		front_ = back_ = nullptr;

	return std::move(*this);
}
