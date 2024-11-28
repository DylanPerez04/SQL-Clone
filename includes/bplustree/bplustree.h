#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include <iomanip>
#include <string>
#include "btree_array_funcs.h"

// save as utf-8
//#pragma execution_character_set( "utf-8" )
//#include <Windows.h>


template <class T> class BPlusTree {
private:
    static const int MINIMUM = 1;
    static const int MAXIMUM = 2 * MINIMUM;

    int tree_size;                       // Number of keys in the datastructure's linked list

    bool dups_ok;                        //true if duplicate keys are allowed
    int data_count;                      //number of data elements
    T data[MAXIMUM + 1];                 //holds the keys
    int child_count;                     //number of children
    BPlusTree<T>* subset[MAXIMUM + 2] = { nullptr };      //subtrees
    BPlusTree<T>* next;

    //true if this is a leaf node
    bool is_leaf() const { return child_count == 0; }

    //return a pointer to this key. NULL if not there.
    T* find_ptr(const T& entry) {
        for (int i = 0; i < data_count; i++)
            if (data[i] == entry) return (data + i);
        return nullptr;
    }

    //entry := leftmost leaf
    void get_smallest(T& entry) {
        //replace entry with the left_most leaf of this tree
        // Keep looking in the first subtree (recursive)
        //  until you get to a leaf.
        // Then, replace entry with data[0] of the leaf node
        const bool debug = false;

        if (is_leaf()) {
            //assert(!empty());
            if(!empty()) entry = data[0];
            return;
        }
        subset[0]->get_smallest(entry);
    }

    /* 
        insert element functions
        allows MAXIMUM+1 data elements in
        the root

        int i = first_ge(data, data_count, entry);
        bool found = (i<data_count && data[i] == entry);

        three cases:
            found
            a. found/leaf: deal with duplicates
            b. found/no leaf: subset[i+1]->loose_insert(entry)
                            fix_excess(i+1) if there is a need

            ! found:
            c. !found / leaf : insert entry in data at position i
            c. !found / !leaf: subset[i]->loose_insert(entry)
                            fix_excess(i) if there is a need

            |   found          |   !found         |
        ------|------------------|------------------|-------
        leaf  |a. dups? +        | c: insert entry  |
            |                  |    at data[i]    |
        ------|------------------|------------------|-------
            | b.               | d.               |
            |subset[i+1]->     | subset[i]->      |
        !leaf | loose_insert()   |  loose_insert()  |
            |fix_excess(i+1)   | fix_excess(i)    |
            |                  |                  |
        ------|------------------|------------------|-------
    */
    void loose_insert(const T& entry) {
        const bool debug = false;

        int i = first_ge(data, data_count, entry);
        bool found = (i < data_count && data[i] == entry);

        if (debug) cout << "loose_insert() : i = " << i << ", found = " << found << endl;
        if (found) {
            if (is_leaf()) { // is duplicate insert
                if (dups_ok) {
                    data[i] = data[i] + entry;
                }
                else {
                    data[i] = entry;
                }
                return;
            }
            // ensures the value of the breadcrumb isn't what is being replaced
            subset[i + 1]->loose_insert(entry);
            fix_excess(i+1);
            return;
        }
        // Not found
        if (is_leaf()) {
            insert_item(data, i, data_count, entry);
            tree_size++;
            return;
        }

        int prior_size = (subset[i] == nullptr ? 0 : subset[i]->tree_size);
        subset[i]->loose_insert(entry);
        fix_excess(i);
        tree_size += subset[i]->tree_size - prior_size;
    }   

    /*
        fix excess in child i
        this node's child i has one too many items: 3 steps:
        1. add a new subset at location i+1 of this node
        2. split subset[i] (both the subset array and the data array) and move half into
        subset[i+1] (this is the subset we created in step 1.)
        3. detach the last data item of subset[i] and bring it and insert
        it into this node's data[]
    
        Note that this last step may cause this node to have too many itesm.
        This is OK. This will be
        dealt with at the higher recursive level. (my parent will fix it!)
    
        NOTE: ODD/EVENNESS
        when there is an excess, data_count will always be odd
        and child_count will always be even.
        so, splitting is always the same.
    
        000 B_PLUS_TREE
        if (subset[i]->is_leaf())
        transfer the middle entry to the right and...
        Deal with next pointers. just like linked list insert
    */
    void fix_excess(int i) {
        const bool debug = false;

        if (subset[i]->data_count > MAXIMUM) {
            insert_item(subset, i + 1, child_count, new BPlusTree<T>(dups_ok));

            if (debug) cout << "fix_excess() : child_count = " << child_count << endl;
            split(subset[i]->data, subset[i]->data_count, subset[i + 1]->data, subset[i + 1]->data_count);
            if (debug) cout << "fix_excess() : subset[i] after split..." << endl << *subset[i] << endl << "subset[i + 1] after split..." << endl << *subset[i + 1] << endl;

            split(subset[i]->subset, subset[i]->child_count, subset[i + 1]->subset, subset[i + 1]->child_count);

            T detached;
            detach_item(subset[i]->data, subset[i]->data_count, detached);
            if (debug) cout << "fix_excess() : detached = " << detached << endl;
            insert_item(data, i, data_count, detached);

            if (subset[i]->is_leaf()) {
                insert_item(subset[i + 1]->data, 0, subset[i + 1]->data_count, detached);
                subset[i + 1]->next = subset[i]->next;
                subset[i]->next = subset[i + 1];
                tree_size++;
            }
        }

    }              

    /* four cases:
          remove element functions:
          allows MINIMUM-1 data elements
          in the root

            leaves:
                a. not found: there is nothing to do
                b. found    : just remove the target
            non leaf:
                c. not found: subset[i]->loose_remove, fix_shortage(i)
                d. found    : subset[i+1]->loose_remove, fix_shortage(i+1) [...]
                    (No More remove_biggest)

            |   !found               |   found                 |
        ------|------------------------|-------------------------|-------
        leaf  |  a: nothing            | b: delete               |
            |     to do              |    target               |
        ------|------------------------|-------------------------|-------
        !leaf | c:                     | d: B_PLUS_TREE          |
            |  [i]->  loose_remove   |   [i+1]-> loose_remove  |
            |  fix_shortage(i)       | fix_shortage(i+1) [...] |
        ------|------------------------|-------------------------|-------


    */
    void loose_remove(const T& entry) {
        const bool debug = false;

        int i = first_ge(data, data_count, entry);
        bool found = (i < data_count && data[i] == entry);

        //if(debug) cout << "loose_remove() : found = " << found << ", i = " << i << endl;

        T found_entry;
        if (found) found_entry = data[i];

        if (is_leaf()) {
            if(debug) cout << "loose_remove() : is_leaf()" << endl;
            if (!found) {
                //[a.] nothing to do
                //entry not in this tree
                if (debug) cout << "loose_remove() : !found && is_leaf()" << endl;
                return;
            }
            else {
                //[b.] just delete the item
                //   my parent will take care of my shortage
                delete_item(data, i, data_count, found_entry);
                tree_size--;
                if(debug) cout << "loose_remove() : deleted " << found_entry << " from data[]!" << endl;
                return;
            }

        }
        else {
            //not leaf:
            // ---- 000 B_PLUS_TREE: no more remove_biggest
            if (!found) {
                //[c.] Not here: subset[i]->loose_remove
                //not found yet. search in the next level:
                int prior_size = subset[i]->tree_size;
                subset[i]->loose_remove(entry);
                tree_size -= prior_size - subset[i]->tree_size;

                fix_shortage(i);

                return;
            }
            else {
                //[d.] found key in an inner node:subset[i+1]->loose_remove
                assert(i < child_count - 1);

                int prior_size = subset[i + 1]->tree_size;
                subset[i + 1]->loose_remove(entry);
                tree_size -= prior_size - subset[i + 1]->tree_size;

                fix_shortage(i + 1);


                T* bread_crumb = nullptr;
                T smallest;

                bread_crumb = find_ptr(found_entry);

                if (bread_crumb == nullptr) {
                    bread_crumb = subset[i]->find_ptr(found_entry);

                    // if fix_shortage called merge_with_next_subset() and bread_crumb was parent of its own entry (bread_crumb already deleted)
                    if (bread_crumb == nullptr) {
                        if (debug) cout << "loose_remove() : bread_crumb could not be found!" << endl;
                        return;
                    }

                    // if fix_shortage called merge_with_next_subset(i - 1) and deleted subset[i + 1], smallest will be found under subset[i].
                    subset[i]->subset[subset[i]->data_count]->get_smallest(smallest);
                    if (debug) cout << "loose_remove() : smallest (was init nullptr) = " << smallest << endl;

                    *bread_crumb = smallest;
                    return;
                    
                }

                subset[i + 1]->get_smallest(smallest);

                if (debug) cout << "loose_remove() : smallest = " << smallest << endl;
                *bread_crumb = smallest;
                return;
                

                /*
                 * Here's The Story:
                 * data[i] is the same as the item that we have deleted.
                 * so, now, it needs to be replaced by the current smallest key
                 *      in subset[i+1]
                 * Problem: After fix_shortage, data[i] could have moved to a different index(?)
                 *      or it could have sunk down to a lower level as a result of merge
                 *      we need to find this item and replace it.
                 *
                 *      Before fix_shortage: we cannot replace data[i] with subset[i+1]->smallest
                 *      before fix_excess because this smallest key could be the very entry
                 *      we are removing. So, we'd be replacing data[i] with entry. and this will
                 *      leave the deleted key in the inner nodes.
                 *
                 *      After fix_shortage: We have no way of knowing where data[i] key (which is
                 *      same as entry) will end up. It could move down one level by the time
                 *      fix_shortage returns.
                 *
                 *      Therefore it seems like we have no choice but to search for it in
                 *      data[ ] AND subset[i]->data[ ]
                 * Current Solution: Kind of messy:
                 *      After fix_shortage(i+1):
                 *      Search for entry in data[ ] and if found, replace it with smallest.
                 *      otherwise, search for it in subset[i]->data[ ]
                 *          and replace it. that's where the key (entry) will end up after
                 *          fix order returns (credit: Jiaruy Li)
                 *
                 */

                //remember. it's possible that data[i] will be gone by the time
                //      fix_shortage returns.
                //key was removed from subset[i+1]:
                //  1. shortage: find entry in data or subset[i+1]->data 
                //              and replace it with subset[i+1]->smallest
                //  2. no shortage: replace data[i] with subset[i+1]->smallest
            }
            // --------------------------------------------------------
        }
    }

    //fix shortage in child i
    // returns a pointer to the fixed_subset
    BPlusTree<T>* fix_shortage(int i) {
        /*
         * fix shortage in subtree i:
         * if child i+1 has more than MINIMUM,
         *          (borrow from right) transfer / rotate left(i+1)
         * elif child i-1 has more than MINIMUM,
         *          (borrow from left) transfer /rotate right(i-1)
         * elif there is a left sibling,
         *          merge with prev child: merge(i-1)
         * else
         *          merge with next (right) child: merge(i)
         *
         *
         * returns a pointer to the fixed_subset
         */
        const bool debug = false;

        assert(i >= 0);
        
        if (is_leaf() || subset[i]->data_count >= MINIMUM) return nullptr;

        if (debug) cout << "fix_shortage() : " << endl;

        if (i + 1 < child_count && subset[i + 1]->data_count > MINIMUM) {
            if (debug) cout << "fix_shortage() : subset[i + 1]->data_count > MINIMUM, so calling transfer_left(i)" << endl;
            transfer_left(i+1);
        }
        else if (i - 1 >= 0 && subset[i - 1]->data_count > MINIMUM) {
            if (debug) cout << "fix_shortage() : subset[i - 1]->data_count > MINIMUM, so calling transfer_right(i)" << endl;
            transfer_right(i-1);
        }
        else if (i - 1 >= 0) {
            if (debug) cout << "fix_shortage() : calling merge_with_next_subset(i-1)" << endl;
            BPlusTree<T>* merged_subset = merge_with_next_subset(i-1);
            if (debug) cout << "fix_shortage() : merged_subset = " << endl << *merged_subset << endl;
        }
        else {
            if (debug) cout << "fix_shortage() : calling merge_with_next_subset(i)" << endl;
            if (i + 1 < child_count) merge_with_next_subset(i);
        }
        if(debug) cout << "fix_shortage() : fixed shortage!" << endl;
        return subset[i];
    }

    BPlusTree<T>* get_smallest_node() {
        if (empty()) return nullptr;
        if (is_leaf())
            return this;
        
        return subset[0]->get_smallest_node();
    }

    //entry := rightmost leaf
    void get_biggest(T& entry) {
        if (is_leaf()) {
            if (data_count > 0) entry = data[data_count - 1];
            return;
        }
        subset[child_count - 1]->get_biggest(entry);
    }

    //remove the biggest child of tree->entry
    void remove_biggest(T& entry) {
        if (is_leaf()) {
            get_biggest(entry);
            remove(entry);
            return;
        }

        subset[child_count - 1]->remove_biggest(entry);
    }
    
    /*
    * transfer one element LEFT from child i
    * --------------------------------------------------------------
    * transfer_left: from subset[i] to subset[i-1]
    *      non-leaf: same as BTree
    *      leaves  : rotate and leave and update data[i-1] with subset[i]->data[0]
    *                  leftmost key after you borrowed goes to replace data[i-1]
    * --------------------------------------------------------------
    *
    * non-leaf keys: BTree's rotate_left
    * ---------------
    *
    * (0 < i < child_count) and (subset[i]->data_count > MINIMUM)
    * subset[i-1] has only MINIMUM - 1 entries.
    *
    * item transfers from child[i] to child [i-1]
    *
    * FIRST item of subset[i]->data moves up to data to replace data[i-1],
    * data[i-1] moves down to the RIGHT of subset[i-1]->data
    *
    *  i = 1:
    *              [50  100]
    *  [  ]         [65 75]       ....
    *   [a]      [b]  [c]  [d]
    *
    *  65 move up to replace 50 (data[i])
    *  65's child (its child 0) moves over to be the future child of 50
    *  50 moves down to the right of subset[i]->data
    *
    *               [65 100]
    *   [50]         [ 75 ]       ....
    * [a]  [b]      [c]  [d]
    *
    * ****This does not belong here:
    * last item of subset[i-1]->data moves up to data to replace data[i],
    * data[i] moves down to the RIGHT of subset[i]->data
    *
    * leaf keys:
    * -------------
    *  fix_shortage(0) calls trnasfer_left(1) so, i = 1
    *          [7 | 10]
    *        /    |     \
    * [( )]<=[(7) | 8]  [10]
    *
    * 1. transfer subset[i(1)]->data[0] to end of subset[i-1]
    *        [7 | 10]
    *      /    |     \
    *   [7]    [8]   [10]
    *
    * 2. update parent of the transfered subset: data[i-1 (0)] = subset[i(1)]->data[0]
    *
    *        [(8) | 10]
    *      /      |     \
    *   [7]     [(8)]   [10]
    *
    *
    *
    */
    void transfer_left(int i) {
        const bool debug = false;

        if(debug) cout << "transfer_left() : " << endl;

        if ((0 < i && i < child_count) &&
            (subset[i]->data_count > MINIMUM && subset[i - 1]->data_count == MINIMUM - 1)) {

            if (!subset[i]->is_leaf()) {
                // BTree rotate_left()
                attach_item(subset[i - 1]->data, subset[i - 1]->data_count, data[i - 1]);

                T to_rotate;
                delete_item(subset[i]->data, 0, subset[i]->data_count, to_rotate);
                data[i - 1] = to_rotate;

                BPlusTree<T>* to_attach = nullptr;
                delete_item(subset[i]->subset, 0, subset[i]->child_count, to_attach);
                attach_item(subset[i - 1]->subset, subset[i - 1]->child_count, to_attach);

                return;
            }
            // subset[i] is a leaf
            T data_to_transfer;
            delete_item(subset[i]->data, 0, subset[i]->data_count, data_to_transfer);
            attach_item(subset[i-1]->data, subset[i-1]->data_count, data_to_transfer);
           
            data[i-1] = subset[i]->data[0];
        }

    }        
   
    /*
    * transfer one element RIGHT from child i
    * --------------------------------------------------------------
    * transfer right from subset[i] to subset[i+1]
    *      non-leaf: same as BTree
    *      leaves  : rotate and leave a 'trace':
    *              data[i] is replaced with subset[i+1]->data[0]
    *              after you borrowed the key,
    * --------------------------------------------------------------
    *
    * non-leaf keys: BTree's rotate_right
    * ---------------
    * (i < child_count - 1) and (subset[i-1]->data_count > MINIMUM)
    * subset[i+ 1] has only MINIMUM - 1 entries.
    *
    * item transfers from child[i] to child [i+1]
    *
    * LAST item of subset[i]->data moves up to data to replace data[i],
    * data[i] moves down to the LEFT of subset[i+1]->data
    *
    * i = 1
    *                     [50 100]
    *      [20 30]        [65 75]          [ ]
    *  [..] [..] [..]   [a] [b] [c]        [..]
    *
    *  75 moves up to replace 100 (data[i])
    *  75's child (its last child) moves over to be the (child 0) child of 100
    *  100 moves down to subset[i]->data
    *
    *                     [50 75]
    *      [20 30]          [65]          [100]
    *  [..] [..] [..]     [a] [b]        [c] [..]
    *
    * last item of subset[i-1]->data moves up to data,
    * data[i] moves down to the RIGHT of subset[i]->data
    *
    * leaf keys:
    * -------------
    *  fix_shortage(2) calls trnasfer_right(1) so, i = 1 subset[i] is ([7|8])
    *  subset[i+1] is the one being fixed.
    *        [7 | 10]
    *      /    |     \
    * [5|6]->[7 | (8)]=>[()]
    *
    * 1. transfer subset[i(1)]->data[last item] to left of subset[i+1]->data
    *        [7 | (10)]
    *      /    |     \
    *   [5|6]->[7]-> [8]
    *
    * 2. update parent of the transfered subset: data[i (1)] = subset[i+1(2)]->data[0]
    *
    *        [7 | 8]
    *      /    |    \
    *   [5|6]->[7]-> [8]
    *
    *
    */
    void transfer_right(int i) {
        const bool debug = false;

        if ((i < child_count - 1) &&
            (subset[i]->data_count > MINIMUM && subset[i + 1]->data_count == MINIMUM - 1)) {

            if (debug) cout << "transfer_right() : " << endl;

            if (!subset[i]->is_leaf()) {
                insert_item(subset[i + 1]->data, 0, subset[i + 1]->data_count, data[i]);

                T to_rotate;
                detach_item(subset[i]->data, subset[i]->data_count, to_rotate);
                data[i] = to_rotate;

                //if (debug) cout << "subset[i]->data_count = " << subset[i]->data_count << endl;
                //if (debug) cout << "subset[i]->child_count = " << subset[i]->child_count << endl;


                BPlusTree<T>* to_attach = nullptr;
                detach_item(subset[i]->subset, subset[i]->child_count, to_attach);
                //if (debug) cout << "subset[i]->child_count = " << subset[i]->child_count << endl;
                insert_item(subset[i + 1]->subset, 0, subset[i + 1]->child_count, to_attach);

                // ----------------------

                T& bread_crumb = subset[i + 1]->data[0];
                subset[i + 1]->subset[1]->get_smallest(bread_crumb);

                // ----------------------

                return;
            }
            // subset[i] is a leaf
            if (debug) cout << "transfer_right() : subset[i (i - 1)] is a leaf" << endl;
            T data_to_transfer;
            detach_item(subset[i]->data, subset[i]->data_count, data_to_transfer);
            insert_item(subset[i + 1]->data, 0, subset[i + 1]->data_count, data_to_transfer);

            data[i] = subset[i + 1]->data[0];
        }

    }       

    /*
    * merge subset i with  i+1
    * ----------------------------------------------------------------------
    *  Merge subset[i] with subset [i+1] REMOVE data[i];
    *  non leaf: same as BTree
    *  leaves  : delete but do not bring down data[i]
    * ----------------------------------------------------------------------
    *
    *   1. remove data[i] from this object
    *   2. if not a leaf, append it to child[i]->data:
    *   3. Move all data items from subset[i+1]->data to right of subset[i]->data
    *   4. Move all subset pointers from subset[i+1]->subset to
    *          right of subset[i]->subset
    *   5. delete subset[i+1] (store in a temp ptr)
    *   6. if a leaf, point subset[i]->next to temp_ptr->next
    *   6. delete temp ptr
    *
    *
    * non-leaf nodes: (same as BTree)
    * ------------------
    *  i = 0:
    *             [50  100]
    *          /      |     \
    *  [  ]         [75]       ....
    *    |         /    \
    *   [a]      [b]    [c]
    *
    *  bring down data[i], merge it with subset[i] and subset[i+1]:
    *      then, get rid of subset[i+1]
    *             [100]
    *            /     \
    *        [50|75]       ....
    *        /  |  \
    *     [a]  [b]  [c]
    *
    *
    *
    * leaf node:
    * ----------------
    * Exactly the same, but do not attach the deleted data[i] to subset[i]->data[ ]
    *
    *  i = 0 : merge 5 and [()]
    *        [7 | 10]
    *      /    |     \
    *  [5]->   [()]->  [10]
    *
    *  Delete data[i] (7), merge subset[i] and subset[i+1]
    *      WITHOUT bringing down data[i]
    *
    *        [10]
    *      /      \
    *  [5]->      [10]
    *
    *
    * i = 1 merge 7 and [()]
    *        [7 | 10]
    *      /    |     \
    *  [5]->   [7]->  [()]
    *
    *  Delete data[i] (10), merge subset[i] and subset[i+1]
    *      WITHOUT bringing down data[i]
    *
    *        [7]
    *      /     \
    *  [5]->      [7]
    *
    *
    *
    *
    */
    BPlusTree<T>* merge_with_next_subset(int i) {
        const bool debug = false;
        
        assert(!is_leaf() && i >= 0 && i < MAXIMUM + 1); // i + 1 < 5

        T parent_item;
        delete_item(data, i, data_count, parent_item);
        if (!subset[i]->is_leaf()) {
            attach_item(subset[i]->data, subset[i]->data_count, parent_item);
        }
        else {
            tree_size--;
        }

        BPlusTree<T>* to_merge = subset[i + 1];
        if (debug) cout << "merge_with_next_subset() : to_merge = " << endl << *to_merge << endl;
        merge(subset[i]->data, subset[i]->data_count, to_merge->data, to_merge->data_count);
        merge(subset[i]->subset, subset[i]->child_count, to_merge->subset, to_merge->child_count);

        subset[i]->next = subset[i + 1]->next;

        delete_item(subset, i + 1, child_count, to_merge);
        delete to_merge;
        to_merge = nullptr;

        if (debug) cout << "merge_with_next_subset() : after merge = " << endl << *subset[i] << endl;
        if(debug) cout << "merge_with_next_subset() : " << "child_count = " << child_count << endl;

        return subset[i];
    } 
public:
    class Iterator {
    private:
        BPlusTree<T>* it;
        int key_ptr;
    public:
        friend class BPlusTree;
        Iterator(BPlusTree<T>* _it = NULL, int _key_ptr = 0) :
            it(_it), key_ptr(_key_ptr) {}

        T operator *() {
            return it->data[key_ptr];
        }

        // postfix (i++)
        Iterator operator++(int un_used) {
            Iterator copy(this->it, this->key_ptr);
            key_ptr = (key_ptr + 1) % it->data_count;
            if (key_ptr == 0) it = it->next;
            return copy;
        }

        // prefix (++i)
        Iterator& operator++() {
            key_ptr = (key_ptr + 1) % it->data_count;
            if (key_ptr == 0) it = it->next;
            return *this;
        }
        friend bool operator ==(const Iterator& lhs, const Iterator& rhs) {
            return (lhs.it == rhs.it && lhs.key_ptr == rhs.key_ptr);
        }
        friend bool operator !=(const Iterator& lhs, const Iterator& rhs) {
            return !(lhs == rhs);
        }
        void print_Iterator() {
            if (it) {
                cout << "iterator: ";
                print_array(it->data, it->data_count, key_ptr);
            }
            else {
                cout << "iterator: NULL, key_ptr: " << key_ptr << endl;
            }
        }
        bool is_null() { return !it; }
        void info() {
            cout << endl << "Iterator info:" << endl;
            cout << "key_ptr: " << key_ptr << endl;
            cout << "it: " << *it << endl;
        }
    };

    //---------------------------------------------------------------------
    //            C T O R  /  B I G   T H R E E
    //---------------------------------------------------------------------

    BPlusTree(bool dups = false) : BPlusTree(nullptr, 0, dups) {}
    BPlusTree(T* a, int size, bool dups = false) : data_count(0), child_count(0), tree_size(0), next(nullptr), dups_ok(dups) {
        assert(size >= 0);
        //SetConsoleOutputCP(65001); // 65001 = UTF-8
        if (a != nullptr) {
            for (int i = 0; i < size; i++) {
                insert(a[i]);
                assert(is_valid());
            }
        }
    }

    BPlusTree(const BPlusTree<T>& other) : data_count(0), child_count(0), next(nullptr), dups_ok(other.dups_ok) {
        copy_tree(other);
    }

    ~BPlusTree() {
        const bool debug = false;
        if (debug) cout << "** destructor fired!" << endl;
        clear_tree();
    }

    BPlusTree<T>& operator =(const BPlusTree<T>& RHS) {
        const bool debug = false;
        if (debug) cout << "** assignment operator fired!" << endl;
        clear_tree();
        copy_tree(RHS);
        return *this;
    }

    //------------------------------------------------
    //          I N S E R T
    //------------------------------------------------
    
    /*
        insert entry into the tree

        in order for this class to be able to keep track of the number of the keys,
                this function (and the functions it calls ) must return a success code.
        If we are to keep track of the number the keys (as opposed to key/values)
            then the success code must distinguish between inserting a new key, or
            adding a new key to the existing key. (for "dupes_ok")

        loose_insert this entry into this root.
        loose_insert(entry) will insert entry into this tree. Once it returns,
            all the subtrees are valid
            btree subtrees EXCEPT this root may have one extra data item:
            in this case (we have excess in the root)
                create a new node, copy all the contents of this root into it,
                clear this root node,
                make the new node this root's only child (subset[0])

        Then, call fix_excess on this only subset (subset[0])
    */
    void insert(const T& entry) {
        const bool debug = false;

        loose_insert(entry);
        if (data_count > MAXIMUM) {
            BPlusTree<T>* copy = new BPlusTree<T>(dups_ok); // shallow copy
            copy_array(copy->data, data, copy->data_count, data_count);
            copy_array(copy->subset, subset, copy->child_count, child_count);
            copy->tree_size = tree_size;

            data_count = 0;
            for (int i = 0; i < child_count; i++)
                subset[i] = nullptr;
            child_count = 0;
            subset[child_count++] = copy;

            if (debug) cout << "insert() : tree after copy..." << endl << *this << endl << "========" << endl << "child_count = " << child_count << endl;

            fix_excess(0);
        }
    }   

    //------------------------------------------------
    //          R E M O V E
    //------------------------------------------------
    /*
    * ---------------------------------------------------------------------------------
    * Same as BTree:
    * Loose_remove the entry from this tree.
    * Shrink if you have to
    * ---------------------------------------------------------------------------------
    * once you return from loose_remove, the root (this object) may have no data and
    * only a single subset:
    * now, the tree must shrink:
    *
    * point a temporary pointer (shrink_ptr) and point it to this root's only subset
    * copy all the data and subsets of this subset into the root (through shrink_ptr)
    * now, the root contains all the data and poiners of it's old child.
    * now, simply delete shrink_ptr, and the tree has shrunk by one level.
    * Note, the root node of the tree will always be the same, it's the
    * child node we delete
    *
    *
    */
    void remove(const T& entry) {

        const bool debug = false;
        loose_remove(entry);
        if (debug) cout << "remove() : child_count after remove = " << child_count << endl;
        if (empty() && child_count == 1) {

            if (debug) cout << "remove() : Root has no data and one subset! " << endl;
            BPlusTree<T>* shrink_ptr = subset[0];

            merge(data, data_count, shrink_ptr->data, shrink_ptr->data_count);
            merge(subset, child_count, shrink_ptr->subset, shrink_ptr->child_count);

            delete_item(subset, 0, child_count, shrink_ptr);

            if (debug) cout << "remove() : child_count after shrink = " << child_count << endl;
            delete shrink_ptr;
            shrink_ptr = nullptr;
        }
        //assert(is_valid());
    }

    //---------------------------------------------------------------
    //      C L E A R   /  C O P Y
    //---------------------------------------------------------------

    //clear this object (delete all nodes etc.)
    void clear_tree() {
        if (is_leaf()) {
            next = nullptr;
            data_count = 0;
            return;
        }

        for (int i = 0; i < child_count; i++) {
            subset[i]->clear_tree();
            delete subset[i];
            subset[i] = nullptr;
        }
        child_count = 0;
        data_count = 0;
        next = nullptr;
        tree_size = 0;
    }        

    void copy_tree(const BPlusTree<T>& other, BPlusTree<T>*& last_node) {
        //assumes this btree is empty. [memory leaks otherwise]
        assert(empty());
        const bool debug = false;
        copy_array(data, other.data, data_count, other.data_count);

        if (!other.is_leaf()) {
            if(debug) cout << "copy_tree() : other isn't a leaf..." << endl;
            subset[0] = new BPlusTree<T>(other.dups_ok);
            subset[0]->copy_tree(*other.subset[0], last_node);

            BPlusTree<T>* prev_child = subset[0];

            for (int i = 1; i < other.child_count; i++) {
                subset[i] = new BPlusTree<T>(other.dups_ok);
                subset[i]->copy_tree(*other.subset[i], last_node);

                if (prev_child->is_leaf()) prev_child->next = subset[i];

                prev_child = subset[i];
            }
        }
        else {
            last_node = this;
        }
        child_count = other.child_count;
        tree_size = other.tree_size;
    }

    //copy other into this object
    void copy_tree(const BPlusTree<T>& other) {
        BPlusTree<T>* temp;
        copy_tree(other, temp);
    }

    //true if entry can be found
    bool contains(const T& entry) const {

        int i = first_ge(data, data_count, entry);
        bool found = (i < data_count && data[i] == entry);

        if (found)
            return true;
        
        if (is_leaf()) return false;

        return subset[i]->contains(entry);
    }
    
    //return a reference to entry
    T& get(const T& entry) {
        //If entry is not in the tree, add it to the tree

        const bool debug = false;
        if (!contains(entry))
            insert(entry);


        return get_existing(entry);
    }              
    //return a reference to entry
    const T& get(const T& entry) const {
        return get_existing(entry); // might cause error (might need to past get_existing into here).
    }

    /*
    * ---------------------------------------------------------------
    * This routing explains plainly how the BPlusTree is organized.
    * i = first_ge
    *       |      found         |    !found         |
    * ------|--------------------|-------------------|
    * leaf  |  you're done       | it's not there    |
    *       |                    |                   |
    * ------|--------------------|-------------------|
    * !leaf | subset[i+1]->get() |  subset[i]->get() |
    *       | (inner node)       |                   |
    * ------|--------------------|-------------------|
    *       |                    |                   |
    *
    * ---------------------------------------------------------------
    *
    */
    T& get_existing(const T& entry) {
        //assert that entry in the tree.
        assert(contains(entry));

        const bool debug = false;
        int i = first_ge(data, data_count, entry);
        bool found = (i < data_count && data[i] == entry);
        if (is_leaf())
            if (found) {
                return data[i];
            }
            else {
                if (debug) cout << "get_existing was called with nonexistent entry" << endl;
                assert(found);
            }
        if (found) //inner node
            return subset[i + 1]->get(entry);
        //or just return true?
        else //not found yet...
            return subset[i]->get(entry);
    }

    //return an iterator to this key.
    Iterator find(const T& key) {
        if (!contains(key)) return NULL;
        Iterator i;
        for (i = this->begin(); !(*i == key); i++) {}
        return i;
    }
    //     NULL if not there.
    //return first that goes NOT BEFORE
    Iterator lower_bound(const T& key) {
        Iterator i;
        for (i = this->begin(); i != this->end() && *i < key; i++) {}
        return i;
    }

    //return first that goes AFTER key
    Iterator upper_bound(const T& key) {
        Iterator i;
        for (i = this->begin(); i != this->end() && *i <= key; i++) {}
        return i;
    }
    //exist or not, the next entry  >entry

    //---------------------------------------------------------------
    //      S I Z E  /  E M P T Y
    //---------------------------------------------------------------
    //count the number of elements
    int size() const {
        //this function counts the number of keys in the btree
        return tree_size;
    }

    //true if the tree is empty
    bool empty() const {
        return (data_count == 0);
    }               

    void print_tree(int level, ostream& outs) const {
        const bool debug = false;
        //1. print the last child (if any)
        //2. print all the rest of the data and children
   
        if (is_leaf()) {
            if(next != nullptr) outs << setw(4 * level) << "" << "^" << endl;

            outs << setw(4 * level) << "" << "⎴" << endl;
            for (int i = data_count - 1; i >= 0; i--)
                outs << setw(4 * level) << "" << data[i] << endl;
            outs << setw(4 * level) << "" << "⎵" << endl;
        }
        else {
            int data_index = data_count - 1;
            for (int i = child_count - 1; i >= 0; i--) {
                subset[i]->print_tree(level + 1, outs);
                if (data_index == data_count - 1) outs << setw(4 * level) << "" << "⎴" << endl;
                if (data_index >= 0) outs << setw(4 * level) << "" << data[data_index] << endl;
                if (data_index == 0) outs << setw(4 * level) << "" << "⎵" << endl;
                data_index--;
            }
        }
    }
    friend ostream& operator<<(ostream& outs, const BPlusTree<T>& print_me) {
        print_me.print_tree(0, outs);
        return outs;
    }

    bool is_valid() {

        //should check that every data[i] < data[i+1]
        for (int i = 1; i < data_count; i++)
            if (data[i - 1] > data[i] || data[i - 1] == data[i]) return false;

        if (is_leaf()) return true;
       

        //data[data_count-1] must be less than equal to 
        //  every subset[child_count-1]->data[ ]

        for (int i = 0; i < subset[child_count - 1]->data_count; i++)
            if (data[data_count - 1] > subset[child_count - 1]->data[i]) return false;

        //every data[i] is greater than every subset[i]->data[ ]
        for (int i = 0; i < data_count; i++)
            for (int j = 0; j < subset[i]->data_count; j++)
                if (data[i] <= subset[i]->data[j]) return false;

        //B+Tree: Every data[i] is equal to subset[i+1]->smallest
        for (int i = 0; i < data_count; i++) {
            T smallest;
            subset[i + 1]->get_smallest(smallest);
            if (!(data[i] == smallest)) return false;
        }


        //Recursively validate every subset[i]
        for (int i = 0; i < child_count; i++)
            if (!subset[i]->is_valid()) return false;

        return true;
    }
    string in_order() {
        string s;

        if (!is_leaf())
            s += subset[0]->in_order();

        for (int i = 0; i < data_count; i++) {
            s += to_string(data[i]) + "|";
            if (!is_leaf()) {
                s += subset[i + 1]->in_order();
            }
        }
        return s;
    }

    string pre_order() {
        string s;

        for (int i = 0; i < data_count; i++) {
            s += to_string(data[i]) + "|";
            if (!is_leaf()) {
                s += subset[i]->pre_order();
            }
        }

        if (!is_leaf())
            s += subset[child_count - 1]->pre_order();

        return s;
    }

    string post_order() {
        string s;

        if (!is_leaf())
            s += subset[0]->post_order();
        
        for (int i = 0; i < data_count; i++) {
            if (!is_leaf()) {
                s += subset[i + 1]->post_order();       
            }
            s += to_string(data[i]) + "|";
        }
        return s;
    }

    //---------------------------------------------------------------------
    //            B E G I N ( )   &   E N D ( )
    //---------------------------------------------------------------------

    Iterator begin() {
        return Iterator(get_smallest_node());
    }
    Iterator end() {
        return Iterator(NULL);
    }

    ostream& list_keys(Iterator from = NULL, Iterator to = NULL) {
        if (from == NULL) from = begin();
        if (to == NULL) to = end();
        for (Iterator it = from; it != to; it++)
            cout << *it << " ";
        return cout;
    }

};

#endif