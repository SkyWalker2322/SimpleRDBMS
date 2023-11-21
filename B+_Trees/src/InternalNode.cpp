#include "InternalNode.hpp"
#include "algorithm"
//creates internal node pointed to by tree_ptr or creates a new one
InternalNode::InternalNode(const TreePtr &tree_ptr) : TreeNode(INTERNAL, tree_ptr) {
    this->keys.clear();
    this->tree_pointers.clear();
    if (!is_null(tree_ptr))
        this->load();
}

//max element from tree rooted at this node
Key InternalNode::max() {
    Key max_key = DELETE_MARKER;
    TreeNode* last_tree_node = TreeNode::tree_node_factory(this->tree_pointers[this->size - 1]);
    max_key = last_tree_node->max();
    delete last_tree_node;
    return max_key;
}

//min element from tree rooted at this node 
Key InternalNode::min() {
    Key min_key = DELETE_MARKER;
    TreeNode* last_tree_node = TreeNode::tree_node_factory(this->tree_pointers[0]);
    min_key = last_tree_node->min();
    delete last_tree_node;
    return min_key;
}

//if internal node contains a single child, it is returned
TreePtr InternalNode::single_child_ptr() {
    if (this->size == 1)
        return this->tree_pointers[0];
    return NULL_PTR;
}

//inserts <key, record_ptr> into subtree rooted at this node.
//returns pointer to split node if exists
//TODO: InternalNode::insert_key to be implemented
TreePtr InternalNode::insert_key(const Key &key, const RecordPtr &record_ptr) {
    TreePtr new_tree_ptr = NULL_PTR;
    cout << "current tree pointer: " << this->tree_ptr << endl; 
    int index = std::lower_bound(this->keys.begin(), this->keys.end(), key) - this->keys.begin();

    TreeNode *child = TreeNode::tree_node_factory(this->tree_pointers[index]);
    TreePtr potential_split_node_ptr = child->insert_key(key, record_ptr);

    if(!is_null(potential_split_node_ptr)) { 
        new_tree_ptr = this->handle_insert_and_split(potential_split_node_ptr, child); 
    }
    this->dump();
    return new_tree_ptr;
}




TreePtr InternalNode::handle_insert_and_split(const TreePtr& child_tree_ptr, TreeNode *child) { 
    TreePtr newInternalTreePtr = NULL_PTR;
    TreeNode* extra_node = TreeNode::tree_node_factory(child_tree_ptr);
    Key key = child->max();

    

    cout << "child's tree pointer = " << child_tree_ptr << " KEY = " << key << endl;

    /* Internal Node splitting condition */
    /**
     * Overflow happens when number of search key values exceed `FANOUT-1`
     * Split the Internal node into two nodes
     * 1st node contains (ceil(FANOUT/2) - 1) values
     * Move the smallest of the remaining keys to the parent
     * 2nd node conatins remaining values
    **/
    if(this->keys.size() < FANOUT - 1) {
        cout << "The available capacity within the current key vector: " << endl; 
        int index = std::upper_bound(this->keys.begin(), this->keys.end(), key) - this->keys.begin();
        cout << "The position at which to insert : " << index << endl;
        this->keys.push_back(key);
        this->tree_pointers.push_back(child_tree_ptr);


        if (index != this->keys.size() - 1) {

            for (int j = this->keys.size() - 1; j > index; j--) {  
                this->keys[j] = this->keys[j - 1];
            }

            for (int j = this->tree_pointers.size() - 1; j > (index + 1); j--) {
                this->tree_pointers[j] = this->tree_pointers[j - 1];
            }

            this->keys[index] = key;
            this->tree_pointers[index + 1] = child_tree_ptr;
        }

        this->size = this->tree_pointers.size();
    } else {
        cout << "Vector size" << endl;
        vector<Key> temp_keys = this->keys;
        vector<TreePtr> temp_tree_pointers = this->tree_pointers;

        int index = std::upper_bound(this->keys.begin(), this->keys.end(), key) - this->keys.begin();

        temp_keys.push_back(key);
        temp_tree_pointers.push_back(child_tree_ptr);


        if (index != temp_keys.size() - 1) {
            for (int j = temp_keys.size() - 1; j > index; j--) {  // keys and datapointer position shifting
                temp_keys[j] = temp_keys[j - 1];
            }

            for (int j = temp_tree_pointers.size() - 1; j > (index + 1); j--) {
                temp_tree_pointers[j] = temp_tree_pointers[j - 1];
            }

            temp_keys[index] = key;
            temp_tree_pointers[index + 1] = child_tree_ptr;
        }

        for(auto it: temp_keys)
        {
            cout << it << " ";
        }
        cout << endl;

        for(auto it: temp_tree_pointers)
        {
            cout << it << " ";
        }
        cout << endl;

        int partition_index = ceil((double)(FANOUT)/(double)(2)) - 1;
        cout << "Partition index : " << partition_index << endl;

        vector<Key> new_keys, new_tree_ptr;

        this->keys.resize(partition_index);
        this->tree_pointers.resize(partition_index+1);
        this->tree_pointers.reserve(partition_index+1);

        for (int i = 0; i < partition_index; i++) {
            this->keys[i] = temp_keys[i];
            this->tree_pointers[i] = temp_tree_pointers[i];
        }

        this->tree_pointers[partition_index] = temp_tree_pointers[partition_index];


        InternalNode* newInternalNode = new InternalNode();
        for (int i = partition_index + 1; i < temp_keys.size(); i++) {
            newInternalNode->keys.push_back(temp_keys[i]);
        }

        for (int i = partition_index + 1; i < temp_tree_pointers.size(); i++) {
            newInternalNode->tree_pointers.push_back(temp_tree_pointers[i]);
        }

        newInternalTreePtr = newInternalNode->tree_ptr;
        cout << "The pointer to the tree within the newly created internal node: : " << newInternalTreePtr << endl;
        newInternalNode->size = temp_tree_pointers.size() - (partition_index + 1);
        cout << "The size of the recently created internal node.: " << newInternalNode->size << endl;
        this->size = this->tree_pointers.size();
        cout << "The size of the existing internal node : " << this->size << endl;
        newInternalNode->dump();
    }

    return newInternalTreePtr;
}



Key InternalNode::getFirstElement() { 
    return this->keys[0];
}

//deletes key from subtree rooted at this if exists
//TODO: InternalNode::delete_key to be implemented
void InternalNode::delete_key(const Key &key, TreePtr left_sibling, TreePtr right_sibling, TreePtr parent_ptr, int parent_index) { 
    cout << "Current_node : " << this->tree_ptr << " left_node : " << left_sibling << " right_node : " << right_sibling;
    cout << " Parent_node : " << parent_ptr << endl;
    int left_sibling_index = -1, right_sibling_index = -1;

    TreePtr child_left_sibling = NULL_PTR, child_right_sibling = NULL_PTR;

    for(int i = 0; i < this->keys.size(); i++) {
        left_sibling_index = i - 1;
        right_sibling_index = i + 1;

        if(key <= this->keys[i]) {
            if(i == 0) {
                child_left_sibling = NULL_PTR;
            } else {
                child_left_sibling = this->tree_pointers[left_sibling_index];
            }
            child_right_sibling = this->tree_pointers[right_sibling_index];
            TreeNode *curr = TreeNode::tree_node_factory(this->tree_pointers[i]);
            cout << "The index of the left sibling node : " << left_sibling_index << ", The index of the right sibling node : " << right_sibling_index << endl;
            curr->delete_key(key, child_left_sibling, child_right_sibling, this->tree_ptr, i);
            break;
        }

        if(i == this->keys.size() - 1) {
            cout << "The index of the sibling node on the left side : " << left_sibling_index << ", The index of the right sibling node : " << right_sibling_index << endl;
            left_sibling_index = i;
            child_right_sibling = NULL_PTR;
            child_left_sibling = this->tree_pointers[left_sibling_index];
            cout << "The sibling located to the left : " << child_left_sibling << ",The sibling located to the right : " << child_right_sibling << endl;
            TreeNode *curr = TreeNode::tree_node_factory(this->tree_pointers[i+1]);
            curr->delete_key(key, child_left_sibling, child_right_sibling, this->tree_ptr, i);
            break;
        }
    }

    cout << endl << " The existing internal node : " << this->tree_ptr << endl;
    
    InternalNode *curr_node = new InternalNode(this->tree_ptr);

    cout << "The size of keys within the current node = " << curr_node->keys.size() << endl;

    if (curr_node->keys.size() >= (FANOUT + 1) / 2 - 1) {
        cout << "A sufficient number of keys within the internal node" << endl;
        return;
    }

    cout << endl << "Insufficient keys in the internal node, leading to underflow " << endl;


    // LEFT BORROW
    if(left_sibling != NULL_PTR) {
        cout << "Attempting to obtain keys from the left sibling" << endl;
        InternalNode *left_node = new InternalNode(left_sibling);
        InternalNode *parent_node = new InternalNode(parent_ptr);
        if(left_node->keys.size() >= ((FANOUT + 1) / 2)) {
            cout << "Obtaining keys from the left sibling :" << endl;
            Key max_key = left_node->keys.back();
            TreePtr max_record_ptr = left_node->tree_pointers.back();
            cout << "Transferring a key from the left sibling to the root : " << max_key << endl;

            int index_to_swap = (right_sibling == NULL_PTR) ? parent_index : (parent_index - 1);

            cout << "Adding the parent key to the node  : " <<  parent_node->keys[index_to_swap] << " at the index " << (index_to_swap) << endl;
            curr_node->keys.insert(curr_node->keys.begin(), parent_node->keys[index_to_swap]);
            cout << "The size of the keys within the current node " << curr_node->keys.size() << endl;

            parent_node->keys[index_to_swap] =  max_key;
            cout << "Inserted " << max_key << " At the index of the parent node : " << (index_to_swap) << endl;
            
            cout << "Inserting the tree pointer " << max_record_ptr << " At the current hierarchical level " << endl;
            curr_node->tree_pointers.insert(curr_node->tree_pointers.begin(), max_record_ptr);

            left_node->keys.pop_back();
            left_node->tree_pointers.pop_back();
            left_node->size = left_node->tree_pointers.size();
            cout << "The size of the new left node after removing a key " << left_node->size << endl;
            left_node->dump();

            parent_node->dump();


            curr_node->size = curr_node->tree_pointers.size();
            cout << "The updated size of the current node " << curr_node->size << endl;
            curr_node->dump();
            return;
        }
    }


    // LEFT MERGE
    if(left_sibling != NULL_PTR) {
        cout << "Attempting to combine with the left sibling node " << endl; 
        InternalNode *left_node = new InternalNode(left_sibling);
        cout << "left sibling: " << left_sibling << endl;
        InternalNode *parent_node = new InternalNode(parent_ptr);

        cout << "The index of the parent node : " << parent_index << endl;

        int parent_borrow_index = -1;

        if(right_sibling == NULL_PTR) {
            parent_borrow_index = parent_index;
        } else {
            parent_borrow_index = parent_index - 1;
        }

        left_node->keys.push_back(parent_node->keys[parent_borrow_index]);

    
        for(auto itr : curr_node->keys) {
            left_node->keys.push_back(itr); 
        }

        for(auto itr : curr_node->tree_pointers) {
            left_node->tree_pointers.push_back(itr); 
        }

        curr_node->keys.resize(0);
        curr_node->tree_pointers.resize(0);


        for(int i = parent_borrow_index; i < parent_node->keys.size()-1; i++) {
            parent_node->keys[i] = parent_node->keys[i+1];
        }


        for(int i = parent_borrow_index + 1; i < parent_node->tree_pointers.size()-1; i++) {
            parent_node->tree_pointers[i] = parent_node->tree_pointers[i+1];
        }


        parent_node->keys.resize(parent_node->keys.size()-1);
        parent_node->tree_pointers.resize(parent_node->tree_pointers.size()-1);

        for(auto it: parent_node->tree_pointers) {
            cout << it << " ";
        }
        cout << endl;

        parent_node->size = parent_node->tree_pointers.size();
        parent_node->dump();

        left_node->size = left_node->tree_pointers.size();
        left_node->dump();

        // delete this;

        return;
    }

    // RIGHT BORROW
    if(right_sibling != NULL_PTR) {
        cout << "Attempting to acquire keys from the right sibling node" << endl;
        cout << "right sibling: " << right_sibling << endl;
        InternalNode *right_node = new InternalNode(right_sibling);
        InternalNode *parent_node = new InternalNode(parent_ptr);
        if(right_node->keys.size() >= ((FANOUT + 1) / 2)) {
            cout << "Taking keys from the right sibling" << endl;
            Key max_key = right_node->keys.front();
            TreePtr max_record_ptr = right_node->tree_pointers.front();
            cout << "Transferring a key from the right sibling to the root = " << max_key << endl;

            cout << "Adding the parent key to the structure  = " <<  parent_node->keys[parent_index] << " at index " << (parent_index) << endl;
            curr_node->keys.push_back(parent_node->keys[parent_index]);
            cout << "The size of keys in the current node " << curr_node->keys.size() << endl;

            parent_node->keys[parent_index] = max_key;
            cout << "inserted " << max_key << " At the index of the parent node = " << (parent_index) << endl;
            
            cout << "inserting tree pointer " << max_record_ptr << " at current level" << endl;
            curr_node->tree_pointers.push_back(max_record_ptr);

            right_node->keys.erase(right_node->keys.begin());
            right_node->tree_pointers.erase(right_node->tree_pointers.begin());
            right_node->size = right_node->tree_pointers.size();
            cout << "The size of the new right node after removing a key " << right_node->size << endl;
            right_node->dump();
            
            parent_node->dump();

            curr_node->size = curr_node->tree_pointers.size();
            cout << "The updated size of the current node " << curr_node->size << endl;
            curr_node->dump();
            return;
        }
    }


    // RIGHT MERGE
    if (right_sibling != NULL_PTR) {
        cout << "Attempting to combine with the right sibling node" << endl; 
        InternalNode *right_node = new InternalNode(right_sibling);
        cout << "right sibling: " << right_sibling << endl;
        InternalNode *parent_node = new InternalNode(parent_ptr);

        curr_node->keys.push_back(parent_node->keys[parent_index]);

        for(auto itr : right_node->keys) {
            curr_node->keys.push_back(itr); 
        }

        for(auto itr : right_node->tree_pointers) {
            curr_node->tree_pointers.push_back(itr); 
        }

        right_node->keys.resize(0);
        right_node->tree_pointers.resize(0);

        for(int i = parent_index; i < parent_node->keys.size()-1; i++) {
            parent_node->keys[i] = parent_node->keys[i+1];
        }

        

        for(int i = parent_index+1; i < parent_node->tree_pointers.size()-1; i++) {
            parent_node->tree_pointers[i] = parent_node->tree_pointers[i+1];
        }

        
        parent_node->keys.resize(parent_node->keys.size()-1);
        parent_node->tree_pointers.resize(parent_node->tree_pointers.size()-1);

        parent_node->size = parent_node->tree_pointers.size();
        parent_node->dump();

        curr_node->size = curr_node->tree_pointers.size();
        curr_node->dump();

        delete right_node;

        return;
    }

}

//runs range query on subtree rooted at this node
void InternalNode::range(ostream &os, const Key &min_key, const Key &max_key) const {
    BLOCK_ACCESSES++;
    for (int i = 0; i < this->size - 1; i++) {
        if (min_key <= this->keys[i]) {
            auto* child_node = TreeNode::tree_node_factory(this->tree_pointers[i]);
            child_node->range(os, min_key, max_key);
            delete child_node;
            return;
        }
    }
    auto* child_node = TreeNode::tree_node_factory(this->tree_pointers[this->size - 1]);
    child_node->range(os, min_key, max_key);
    delete child_node;
}

//exports node - used for grading
void InternalNode::export_node(ostream &os) {
    TreeNode::export_node(os);
    for (int i = 0; i < this->size - 1; i++)
        os << this->keys[i] << " ";
    os << endl;
    for (int i = 0; i < this->size; i++) {
        auto child_node = TreeNode::tree_node_factory(this->tree_pointers[i]);
        child_node->export_node(os);
        delete child_node;
    }
}

//writes subtree rooted at this node as a mermaid chart
void InternalNode::chart(ostream &os) {
    string chart_node = this->tree_ptr + "[" + this->tree_ptr + BREAK;
    chart_node += "size: " + to_string(this->size) + BREAK;
    chart_node += "]";
    os << chart_node << endl;

    for (int i = 0; i < this->size; i++) {
        auto tree_node = TreeNode::tree_node_factory(this->tree_pointers[i]);
        tree_node->chart(os);
        delete tree_node;
        string link = this->tree_ptr + "-->|";

        if (i == 0)
            link += "x <= " + to_string(this->keys[i]);
        else if (i == this->size - 1) {
            link += to_string(this->keys[i - 1]) + " < x";
        } else {
            link += to_string(this->keys[i - 1]) + " < x <= " + to_string(this->keys[i]);
        }
        link += "|" + this->tree_pointers[i];
        os << link << endl;
    }
}

ostream& InternalNode::write(ostream &os) const {
    TreeNode::write(os);
    for (int i = 0; i < this->size - 1; i++) {
        if (&os == &cout)
            os << "\nP" << i + 1 << ": ";
        os << this->tree_pointers[i] << " ";
        if (&os == &cout)
            os << "\nK" << i + 1 << ": ";
        os << this->keys[i] << " ";
    }
    if (&os == &cout)
        os << "\nP" << this->size << ": ";
    os << this->tree_pointers[this->size - 1];
    return os;
}

istream& InternalNode::read(istream& is) {
    TreeNode::read(is);
    this->keys.assign(this->size - 1, DELETE_MARKER);
    this->tree_pointers.assign(this->size, NULL_PTR);
    for (int i = 0; i < this->size - 1; i++) {
        if (&is == &cin)
            cout << "P" << i + 1 << "= ";
        is >> this->tree_pointers[i];
        if (&is == &cin)
            cout << "K" << i + 1 << "= ";
        is >> this->keys[i];
    }
    if (&is == &cin)
        cout << "P :" << this->size;
    is >> this->tree_pointers[this->size - 1];
    return is;
}
