#include "RecordPtr.hpp"
#include "LeafNode.hpp"
#include "InternalNode.hpp"

LeafNode::LeafNode(const TreePtr &tree_ptr) : TreeNode(LEAF, tree_ptr) {
    this->data_pointers.clear();
    this->next_leaf_ptr = NULL_PTR;
    if(!is_null(tree_ptr))
        this->load();
}

//returns max key within this leaf
Key LeafNode::max() {
    auto it = this->data_pointers.rbegin();
    return it->first;
}



Key LeafNode::min() {                 
    auto it = this->data_pointers.begin();
    return it->first;
}

//inserts <key, record_ptr> to leaf. If overflow occurs, leaf is split
//split node is returned
//TODO: LeafNode::insert_key to be implemented
TreePtr LeafNode::insert_key(const Key &key, const RecordPtr &record_ptr) {       
    TreePtr new_leaf = NULL_PTR;
    /* Leaf Node splitting condition */
    /**
     * Overflow happens when number of search key values exceed `FANOUT-1`
     * Split the leaf node into two nodes
     * 1st node contains ceil((FANOUT-1)/2) values
     * 2nd node conatins remaining values
     * copy the smallest search key value of the 2nd node to the parent node
    **/
    if(this->data_pointers.size() == FANOUT) {
        map<Key, RecordPtr> temp_data_pointers;

        for(auto it: this->data_pointers)
        {
            temp_data_pointers.insert(make_pair(it.first, it.second));
        }

        temp_data_pointers.insert(make_pair(key, record_ptr));

        this->data_pointers.clear();
        int no_of_elem_in_first_map = ceil((double)(FANOUT)/(double)(2));
        int no_of_elem_in_second_map = (temp_data_pointers.size() - no_of_elem_in_first_map);

        int k = 0;

        while(k < no_of_elem_in_first_map)
        {
            Key k1 = temp_data_pointers.begin()->first;
            RecordPtr rec = temp_data_pointers.begin()->second;
            this->data_pointers.insert(make_pair(k1, rec));
            temp_data_pointers.erase(temp_data_pointers.begin());
            k++;
        }

        this->size = no_of_elem_in_first_map;

        LeafNode *new_leaf_2 = new LeafNode();

        int l = 0;

        while(l < no_of_elem_in_second_map)
        {
            Key k1 = temp_data_pointers.begin()->first;
            RecordPtr rec = temp_data_pointers.begin()->second;
            new_leaf_2->data_pointers.insert(make_pair(k1, rec));
            temp_data_pointers.erase(temp_data_pointers.begin());
            l++;
        }

        new_leaf_2->size = no_of_elem_in_second_map;

        this->next_leaf_ptr = (new_leaf_2->tree_ptr);
        this->dump();
        new_leaf_2->dump();

        return new_leaf_2->tree_ptr;
    } else {
        this->data_pointers.insert(make_pair(key, record_ptr));
        this->size++;
        this->dump();
    }
    
    // this->dump();
    return new_leaf;
}


void LeafNode::dump() const{                   
    ofstream fout(TEMP_PATH + this->tree_ptr);
    this->write(fout);
    fout.close();
}

void LeafNode::load() {                       
    ifstream fin(TEMP_PATH + this->tree_ptr);
    this->read(fin);
    fin.close();
}

//key is deleted from leaf if exists
//TODO: LeafNode::delete_key to be implemented
void LeafNode::delete_key(const Key &key, TreePtr left_sibling, TreePtr right_sibling, TreePtr parent_ptr, int parent_index) { 
    
    cout << "Current_node : " << this->tree_ptr << " left_node : " << left_sibling << " right_node :" << right_sibling;
    cout << " Parent_node : " << parent_ptr << ", Parent_index: " << parent_index << endl;
    
    int pos = -1;

    if(this->data_pointers.find(key) == this->data_pointers.end())
        return;

    cout << "Removing or deleting a key from a data structure : " << key << endl;
    this->data_pointers.erase(key);

    this->size = this->data_pointers.size();
    this->dump();

    if(this->data_pointers.size() >= (FANOUT + 1) / 2) {
        return;
    }


    // LEFT BORROW
    if(left_sibling != NULL_PTR) {
        LeafNode *left_node = new LeafNode(left_sibling);

        if(left_node->data_pointers.size() >= ((FANOUT + 1) / 2) + 1) {
            cout << "Taking keys or elements from the left side" << endl;
            cout << "The left node contains a sufficient number of keys" << endl;
            Key max_key = left_node->data_pointers.rbegin()->first;
            RecordPtr max_record_ptr = left_node->data_pointers.rbegin()->second;

            cout << "got max key = " << max_key << " record_ptr = " << max_record_ptr << endl;

            this->data_pointers[max_key] = max_record_ptr;

            left_node->data_pointers.erase(max_key);
            left_node->size = left_node->data_pointers.size();
            cout << "The size of the new left node = " << left_node->size << endl;
            left_node->dump();

            if(parent_ptr != NULL_PTR) {
                InternalNode* parent_node = new InternalNode(parent_ptr);
                /* edge case, at rightmost */
                int replace_index = (right_sibling == NULL_PTR)? (parent_index) : (parent_index - 1);
                parent_node->keys[replace_index] = left_node->data_pointers.rbegin()->first;
                cout << "setting the key " << left_node->data_pointers.rbegin()->first << " At the specified index within the parent node " << replace_index << endl;
                parent_node->dump();
            }
            
            this->size = this->data_pointers.size();
            cout << "The updated size of the current node = " << this->size << endl;

            this->dump();
            return;
        }
    }


    // LEFT MERGE
    if(left_sibling != NULL_PTR) {
        cout << "Attempting to combine (merge) with the left sibling node" << endl;
        LeafNode* left_node = new LeafNode(left_sibling);
        InternalNode* parent_node = new InternalNode(parent_ptr);
        cout << "Moving the values from the current node to the left sibling node" << endl;
        for(auto itr : this->data_pointers) {
            Key key = itr.first;
            RecordPtr rp = itr.second;
            left_node->data_pointers[key] = rp; 
        }
        cout << "new left size = " << left_node->data_pointers.size() << endl;

        cout << "Adjusting the next pointer of the left sibling node to match the next pointer of the current node" << endl;
        cout << "current next = " << this->next_leaf_ptr << endl;
        left_node->next_leaf_ptr = this->next_leaf_ptr;
        cout << "The pointer to the node to the left, typically in the context of sibling nodes = " << left_node->next_leaf_ptr << endl; 

        int start = (right_sibling == NULL_PTR)? (parent_node->keys.size()-1) : (parent_index - 1);
        cout << "Removing (deleting) the key from the parent at a specific index = " << start << endl;
        for (int i = start; i < parent_node->keys.size()-1; i++) {
            parent_node->keys[i] = parent_node->keys[i + 1];
        }
        parent_node->keys.resize(parent_node->keys.size() - 1);

        int start_rp = (right_sibling == NULL_PTR)? (parent_node->tree_pointers.size()-1) : (parent_index);
        cout << "Removing (deleting) the block pointer from the parent at a specific index " << start_rp << endl;
        for(int i = start_rp; i < parent_node->tree_pointers.size()-1; i++) {
            parent_node->tree_pointers[i] = parent_node->tree_pointers[i+1];
        }

        parent_node->tree_pointers.resize(parent_node->tree_pointers.size() - 1);

        parent_node->size = parent_node->tree_pointers.size();
        cout << "The updated size of the parent node : " << parent_node->size << endl;
        parent_node->dump();

        left_node->size = left_node->data_pointers.size();
        cout << "The updated size of the left node after an operation: " << left_node->size << endl;
        left_node->dump();

        return;
    }


    // RIGHT BORROW
    if(right_sibling != NULL_PTR) {
        LeafNode *right_node = new LeafNode(right_sibling);

        if(right_node->data_pointers.size() >= ((FANOUT + 1) / 2) + 1) {
            cout << "Taking keys or elements from the right side" << endl;
            cout << "The right node contains a sufficient number of keys" << endl;
            
            Key max_key = right_node->data_pointers.begin()->first;
            RecordPtr max_record_ptr = right_node->data_pointers.begin()->second;
            cout << "Obtained the maximum key value = " << max_key << " The pointer to a record or data associated with a specific key or entry = " << max_record_ptr << endl;
            
            this->data_pointers[max_key] = max_record_ptr;

            right_node->data_pointers.erase(max_key);
            right_node->size = right_node->data_pointers.size();
            right_node->dump();

            cout << "The size of the right node after a merge or other operation = " << right_node->size << endl;

            if(parent_ptr != NULL_PTR) {
                InternalNode* parent_node = new InternalNode(parent_ptr);
                parent_node->keys[parent_index] = max_key;
                cout << "setting key " << max_key << " in parent index " << (parent_index) << endl;
                parent_node->dump();
            }

            this->size = this->data_pointers.size();
            cout << "The updated size of the current node = " << this->size << endl;
            this->dump();

            return;
        }
    }


    // RIGHT MERGE
    if (right_sibling != NULL_PTR) {
        cout << "Attempting to combine (merge) with the right sibling node" << endl;
        LeafNode* right_node = new LeafNode(right_sibling);
        InternalNode* parent_node = new InternalNode(parent_ptr);
        cout << "Moving the values from the right sibling node to the current node" << endl;
        for(auto itr : right_node->data_pointers) {
            Key key = itr.first;
            RecordPtr rp = itr.second;
            this->data_pointers[key] = rp; 
        }
        cout << "The updated size of the current node = " << this->data_pointers.size() << endl;

        cout << "Adjusting the next pointer of the current node to match the next pointer of its right sibling" << endl;
        cout << "The pointer to the right neighboring leaf node = " << right_node->next_leaf_ptr << endl;
        this->next_leaf_ptr = right_node->next_leaf_ptr;
        cout << "The pointer to the left neighbor of the current node = " << this->next_leaf_ptr << endl; 

        cout << "Removing (deleting) the key from the parent at a specific index = " << parent_index << endl;
        for (int i = parent_index; i < parent_node->keys.size()-1; i++) {
            parent_node->keys[i] = parent_node->keys[i + 1];
        }
        parent_node->keys.resize(parent_node->keys.size() - 1);

        cout << "The size of the parent node: " << parent_node->size << endl;
        cout << "The size of the tree pointer within the parent node : " << parent_node->tree_pointers.size() << endl;
        cout << "Removing (deleting) the block pointer from the parent at a specific index " << (parent_index + 1) << endl;
        for(int i = parent_index + 1; i < parent_node->tree_pointers.size()-1; i++) {
            parent_node->tree_pointers[i] = parent_node->tree_pointers[i+1];
        }
        parent_node->tree_pointers.resize(parent_node->tree_pointers.size() - 1);

        parent_node->size = parent_node->tree_pointers.size();
        cout << "The updated size of the keys within the parent node : " << parent_node->keys.size() << endl;
        cout << "The updated size of the parent node : " << parent_node->size << endl;
        parent_node->dump();

        this->size = this->data_pointers.size();
        cout << "The updated size of the current node : " << this->size << endl;
        this->dump();

        return;
    }

}

//runs range query on leaf
void LeafNode::range(ostream &os, const Key &min_key, const Key &max_key) const {
    BLOCK_ACCESSES++;
    for(const auto& data_pointer : this->data_pointers){
        if(data_pointer.first >= min_key && data_pointer.first <= max_key)
            data_pointer.second.write_data(os);
        if(data_pointer.first > max_key)
            return;
    }
    if(!is_null(this->next_leaf_ptr)){
        auto next_leaf_node = new LeafNode(this->next_leaf_ptr);
        next_leaf_node->range(os, min_key, max_key);
        delete next_leaf_node;
    }
}


Key LeafNode::getFirstElement() {
    return this->data_pointers.begin()->first;
}

//exports node - used for grading
void LeafNode::export_node(ostream &os) {
    TreeNode::export_node(os);
    for(const auto& data_pointer : this->data_pointers){
        os << data_pointer.first << " ";
    }
    os << endl;
}

//writes leaf as a mermaid chart
void LeafNode::chart(ostream &os) {
    string chart_node = this->tree_ptr + "[" + this->tree_ptr + BREAK;
    chart_node += "size: " + to_string(this->size) + BREAK;
    for(const auto& data_pointer: this->data_pointers) {
        chart_node += to_string(data_pointer.first) + " ";
    }
    chart_node += "]";
    os << chart_node << endl;
}

ostream& LeafNode::write(ostream &os) const {
    TreeNode::write(os);
    for(const auto & data_pointer : this->data_pointers){
        if(&os == &cout)
            os << "\n" << data_pointer.first << ": ";
        else
            os << "\n" << data_pointer.first << " ";
        os << data_pointer.second;
    }
    os << endl;
    os << this->next_leaf_ptr << endl;
    return os;
}

istream& LeafNode::read(istream& is){
    TreeNode::read(is);
    this->data_pointers.clear();
    for(int i = 0; i < this->size; i++){
        Key key = DELETE_MARKER;
        RecordPtr record_ptr;
        if(&is == &cin)
            cout << "K: ";
        is >> key;
        if(&is == &cin)
            cout << "P: ";
        is >> record_ptr;
        this->data_pointers.insert(pair<Key,RecordPtr>(key, record_ptr));
    }
    is >> this->next_leaf_ptr;
    return is;
}