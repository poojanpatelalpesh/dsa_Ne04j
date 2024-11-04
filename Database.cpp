#include <iostream>
#include <unordered_map>
#include <string>
#include <sstream> // For stringstream
#include <iomanip> // For std::setw and std::setfill
#include <unordered_set>
#include <vector>
#include <algorithm>

using namespace std;


class Node {
public:
    // Each node represents an entity. The label indicates the type of entity,
    // such as "Person", "Car", "Organization", etc.
    string label;

    // Unique identifier for the node, typically a name or ID.
    string name;

    // A map to store various properties of the node.
    // The key is the property name (string) and the value is the property value (string).
    unordered_map<string, string> properties;

    // Constructor to initialize a node with a label and name.
    Node(const string& label, const string& name) : label(label), name(name) {}

    // Function to add or update a property. This method allows setting properties
    // dynamically based on key-value pairs, making it flexible for different entities.
    void updateProperty(const string& key, const string& value) {
        properties[key] = value;
    }

    // Function to get a specific property by key.
    // If the property exists, it prints its value in a formatted JSON style.
    void getProperty(const string& key) const {
        auto it = properties.find(key);
        if (it != properties.end()) {
            cout << "\"" << key << "\": \"" << it->second << "\"" << endl;
        } else {
            cout << "\"" << key << "\": null" << endl; // If the property does not exist
        }
    }

    // Function to print all node properties in a readable JSON format.
    void printProperties() const {
        cout << "{" << endl;
        cout << "  \"Label\": \"" << label << "\"," << endl;
        cout << "  \"Name\": \"" << name << "\"," << endl;

        // Print all properties stored in the unordered_map
        cout << "  \"Properties\": {" << endl;

        // Use an iterator to keep track of whether we're at the last element
        auto it = properties.begin();
        for (size_t i = 0; i < properties.size(); ++i, ++it) {
            cout << "    \"" << it->first << "\": \"" << it->second << "\"";
            // Check if we're not at the last element
            if (i < properties.size() - 1) {
                cout << ",";
            }
            cout << endl;
        }
        cout << "  }" << endl;
        cout << "}" << endl;
    }

    // Function to delete a specific property by key.
    // This method removes the property if it exists in the properties map.
    void deleteProperty(const string& key) {
        auto it = properties.find(key);
        if (it != properties.end()) {
            properties.erase(it); // Remove the property from the map
            cout << "Property \"" << key << "\" deleted." << endl;
        } else {
            cout << "Property \"" << key << "\" does not exist." << endl; // If the property does not exist
        }
    }

    // Function to delete all properties.
    // This method clears the properties map, removing all properties associated with the node.
    void clearProperties() {
        properties.clear(); // Remove all properties from the map
        cout << "All properties cleared." << endl;
    }
};



class Relationship
{ 
    public:

    // this class made a relation between two nodes.relation like friends,purchased,likes etc.
    string relation;
    //[what if there are more then one type].

    // properties of relationship
    unordered_map<string, string> properties;

    // constructor
    Relationship(string relation)
    {
        this->relation = relation;
    }

    void updateProperty(const string key, const string value)
    {
        properties[key] = value;
    }

    void printRelation()
    {
        cout << "Relationship:" << relation << endl;
    }
};

// now we need to connect different nodes via realtion using graph.
class Graph
{

    // making a list of all node with there name(id). using hash map.
    unordered_map<string, Node *> nodes;

    // Index for fast queries by label
    unordered_map<string, unordered_set<Node *>> labelIndex;
    //[what if someone want all nodes with purticular properties?].

    // Map of maps to store relationships, keyed by "from node name", then "to node name"
    unordered_map<string, unordered_map<string, Relationship*>> edges;

public:
    void addNode(const string label, const string name)
    {
        /// adding node into nodes.
        Node *newNode = new Node(label, name);
        nodes[name] = newNode;
        //[what if two person have same name?]

        // also adding in labelIndex.
        labelIndex[label].insert(newNode); // if label already exixt  then it will add into that set otherwise it will create new pair.
    }

    void addNodeProperty(const string label, const string name, const string key, const string value)
    { // key is property name and value is that property
        if (nodes.find(name) != nodes.end())
        {                                            // to find if node exists in graph or not
            nodes[name]->updateProperty(key, value); // updating property of node
        }
        else
        {
            cout << "Node not exists" << endl;
        }
        //[what if i update labelIndex here for all properties?]
    }

    void addRelationship(string name1, string name2, string relation)
    {
        if (nodes.find(name1) != nodes.end() && nodes.find(name2) != nodes.end())   // to find if name1 and name2 exists in graph or not
        { 
            Relationship* type = new Relationship(relation);

            edges[name1][name2]=type;

            //[what if two same node have multiple relationship?].

        }
        else
        {
            cout << "data not matched";
        }
    }

     void addRelationProperty(const string name1, const string name2, const string key, const string value)
    { // key is property name and value is that property
        if (edges.find(name1) != edges.end() && edges.find(name2) != edges.end())      // to find if edge exists in graph or not
        {                                                                               
                Relationship* type = edges[name1][name2];
                type->updateProperty(key,value); // updating property of relationship
        }
        else
        {
            cout << "No relation found" << endl;
        }

    }

    Node* getNodeByName(const string& label,const string& name){
         if (nodes.find(name) != nodes.end())    // to find if node exists in graph or not
        {                                           
              return nodes[name]; //[what different label have same name?]
        }

        else{
           cout << "Node with name " << name << " not found!" << endl;
           return nullptr;
        }
    }

    vector<Node*> getNodesByLabel(const string& label){
        vector<Node*> nodesWithSameLabel;
        if(labelIndex.find(label) != labelIndex.end()){
             for(auto node:labelIndex[label]){
                nodesWithSameLabel.push_back(node);
             }

        }
        else{
            cout << "label not exist." ;
            
        }
        return nodesWithSameLabel;
    }

   // [what if we want to retrive nodes with specific property like  marks,age,etc?];
   
   void RetrieveRelatedNodes(const string& name,const string& relation){       //[what if we want to retrive all connected node irrespective of relationship?];
       if(edges.find(name)!=edges.end()){
        //iterating over nodes that are connected to given name.
         cout<<"related Nodes to " <<name<<" are : ";
            for(const auto& it: edges[name]){
                 //[what if iteration over all different kind of relationships of two connected nodes?]
                 if(it.second->relation==relation){  //if relation match with given relation.
                    cout<<it.first<<", ";
                 }
            }
            cout<<endl;
         
       }
       else{
        cout <<"name:"<<name<<" not exist.";
       }    
       
   }

   void getNodeProperty(const string& name){

                 if(nodes.find(name)!=nodes.end()){
                       nodes[name]->printProperties();

                 }
                 else{
                    cout<<name<<" not exist in Database."<<endl;
                 }
   }
             
   
   
   void getRelation(const string& name1, const string& name2){
    
    //checking name1 or name2 exists or not.
          if(edges.find(name1)!=edges.end() && edges[name1].find(name2)!=edges[name2].end()){
                     
             return edges[name1][name2]->printRelation();
          }
          else{
            cout<<"Relation not exists";
          }

   }

   void deleteNode(const string label, const string name){
             if(nodes.find(name)!=nodes.end()){
                // 1. Remove all outgoing relationships from this node
                  edges.erase(name);

               // 2. Remove all incoming relationships to this node
                for (auto &it : edges) {
                       it.second.erase(name);
                    }
             
              // 3.remove node from labelIndex.
              if (labelIndex.find(label) != labelIndex.end()) {
                          labelIndex[label].erase(nodes[name]);  // Remove the node from its label set.

                     // If no nodes remain with this label, erase the label itself
                        if (labelIndex[label].empty()) {
                        labelIndex.erase(label);
                       }
                }
             
              // 4. Remove the node from nodes map
                  delete nodes[name]; // Clean up the dynamically allocated memory
                  nodes.erase(name);

                 }
            else cout<<"Node not found";
             
   }

   void deleteReation(const string& name1, const string& name2, const string& relationtype ){
          //checking name1 or name2 exists or not.
          if(edges.find(name1)!=edges.end() && edges[name1].find(name2)!=edges[name2].end()){
            //getting relationship
             Relationship* rel = edges[name1][name2];
             //checking if relation type match or not.
             if(rel->relation==relationtype){ 
                 //first clearing all properties a relation have
                 rel->properties.clear();

                 //now deleting relation itself
                 delete rel;
                 
                 edges[name1].erase(name2);

                 //[what if name1 only have one relation with name2?]
                 if (edges[name1].empty()) {
                         edges.erase(name1);
                     }

                //[what if name1 and name2 have multiple relation]
             }
            else{
               cout<<"rlation between "<<name1<<" and "<<name2<<" is not of type "<<relationtype<<endl;
            }

          }
          else {
             cout<<"Nodes not found"<<endl;
          }
    
   }
   //[what if we want to delete all relationship?]

   void interpretQuery(string query){

   }
   
};

int main()
{
    Graph g;

    while(true){

    string query;
    getline(cin,query);

    if(query=="end") {break;}
    else{     
        g.interpretQuery(query);
    }


   }
   
    return 0;
}
