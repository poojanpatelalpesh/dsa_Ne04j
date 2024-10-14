#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>

using namespace std;

class Node
{
public:
    // each node is a entity first we need label of entity it is just a type like person,car,organization.
    // creating a label
    string label;

    // uniqe indentity for each person.
    string name;

    //[node may have more then one label?]

    /*each node have some properties like if node is "person->then it haveage,height, salary, proffesion, etc. as properties"
                                          if it is "product->then it have product_id,category,price,brand,stock_quantity,weight,
                                          dimensions,color,material,warranty etc. as properties"
    */
    // we are storing all that properties using unordered map or hash map.
    unordered_map<string, string> properties;
    //[what if we need to store integer value and we add them in future for two different nodes?]

    // constructor
    Node(string label, string name)
    {
        this->label = label;
        this->name = name;
    }

    // Function to add or update a property
    void updateProperty(string key, string value)
    { // passing by reference
        properties[key] = value;
    }

    // printing all node properties.
    void printProperty()
    {
        cout << "label:" << label << endl;

        for (auto &pair : properties)
        {
            cout << pair.first << ": " << pair.second << endl;
        }
    }
    //[what if someone wants specific property?]
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

   void getNodeProperty(){

   }
   void getRelation(const string& name1, const string& name2){
       //checking existence of name1 or name2
       if(edges.find(name1)!=edges.end() && edges[name1].find(name2)!=edges[name2].end())
       {
           return edges[name1][name2]->printRelation();
       }
       else
       {
           cout << "Relation does not exist";
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
     
   
     
};

int main()
{

    Graph g;

    //testing

    

    return 0;
}
