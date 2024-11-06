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
    // This class represents a relationship between two nodes, like "friends", "purchased", "likes", etc.
    string relation;

    // Properties of the relationship
    unordered_map<string, string> properties;

    // Constructor to initialize a relationship with a specific relation type
    Relationship(const string& relationType)
        : relation(relationType) {}

    // Method to update or add a property of the relationship
    void setProperty(const string& key, const string& value)
    {
        properties[key] = value;
    }

    // Method to remove a specific property by key
    void removeProperty(const string& key)
    {
        auto it = properties.find(key);
        if (it != properties.end()) {
            properties.erase(it); // Remove the specific property
            cout << "Property \"" << key << "\" has been removed." << endl;
        } else {
            cout << "Property \"" << key << "\" does not exist." << endl;
        }
    }

    // Method to clear all properties from the relationship and provide feedback
    void clearProperties()
    {
        properties.clear();
        cout << "All properties have been cleared." << endl;
    }

    // Method to retrieve a property value by key
    string getProperty(const string& key) const
    {
        auto it = properties.find(key);
        if (it != properties.end()) {
            return it->second; // Return the property value if found
        }
        return ""; // Return empty if not found
    }

    // Method to print the relationship information
    void displayRelationship() const
    {
        cout << "Relationship: " << relation << endl;
        if (!properties.empty()) {
            cout << "Properties: {" << endl;
            for (auto it = properties.begin(); it != properties.end(); ++it) {
                cout << "  \"" << it->first << "\": \"" << it->second << "\"";
                if (std::next(it) != properties.end()) {
                    cout << ","; // Add comma only if it's not the last element
                }
                cout << endl;
            }
            cout << "}" << endl;
        } else {
            cout << "No properties." << endl; // Indicate if no properties exist
        }
    }
};

class Graph
{

    // Maps each node's unique name to its Node object for easy lookup.
    unordered_map<string, Node*> nodes;
   
    // Provides a label-based index for fast lookup of nodes by type.
    // Each label (e.g., "Person") maps to a set of Node pointers that have that label.
    unordered_map<string, unordered_set<Node*>> labelIndex;


    // Map of maps to store relationships, keyed by "from node name", then "to node name"
    unordered_map<string, unordered_map<string, Relationship*>> edges;

public:
    
    void addNode(const string& label, const string& name) {
    // Check if a node with the same name already exists
    if (nodes.find(name) != nodes.end()) {
        cout << "Error: A node with the name \"" << name << "\" already exists." << endl;
        return;
    }

    // Create a new Node and add it to the nodes map
    Node* newNode = new Node(label, name);
    nodes[name] = newNode;

    // Insert the node into labelIndex for efficient lookup by label
    labelIndex[label].insert(newNode); 
  }

    void addNodeProperty(const string& name, const string& key, const string& value) {
    // Check if node exists in the graph using its unique name
    auto it = nodes.find(name);
    if (it != nodes.end()) {
        it->second->updateProperty(key, value); // Update node's property
    } else {
         cout << "Error: Entity with name \"" << name << "\" does not exist." << endl;
    }
  }

    void getNodeProperty(const string& name, const vector<string>& keys) {
    // Check if the node exists in the graph
    if (nodes.find(name) == nodes.end()) {
        cout << "Error: Node \"" << name << "\" does not exist in the database." << endl;
        return;
    }

    Node* node = nodes[name]; // Retrieve the node

    // If "ALL" is specified in the keys, print all properties
    if (keys.size() == 1 && keys[0] == "ALL") {
        node->printProperties();
        return;
    }

    // If specific keys are provided, iterate through each key and print each property
    cout << "{\n";
    cout << "  \"Name\": \"" << name << "\",\n";
    cout << "  \"Properties\": {\n";

    bool firstProperty = true;
    for (const auto& key : keys) {
        if (!firstProperty) {
            cout << ",\n";
        }
        cout << "    ";
        node->getProperty(key); // Display each property
        firstProperty = false;
    }

    cout << "\n  }\n";
    cout << "}" << endl;
   }
  
    void deleteNodeProperty(const string& name, const vector<string>& keys) {
    // Check if node exists
    if (nodes.find(name) == nodes.end()) {
        cout << "Error: Node '" << name << "' not found in the database." << endl;
        return;
    }

    Node* node = nodes[name];

    // Check if the keys contain "ALL"
    if (keys.size() == 1 && keys[0] == "ALL") {
        // Clear all properties if "ALL" is specified
        node->clearProperties();
        cout << "All properties for node '" << name << "' have been cleared." << endl;
        return;
    }

    // Iterate through each key and delete if it exists
    bool anyKeyFound = false;
    for (const string& key : keys) {
        if (node->properties.find(key) != node->properties.end()) {
            // Delete the specific property if it exists
            node->deleteProperty(key);
            anyKeyFound = true;
        } else {
            cout << "Warning: Property '" << key << "' not found for node '" << name << "'." << endl;
        }
    }

    // If none of the specified keys were found, notify the user
    if (!anyKeyFound) {
        cout << "Error: None of the specified properties were found for node '" << name << "'." << endl;
    } else {
        cout << "Specified properties for node '" << name << "' have been deleted where found." << endl;
    }
}

    void getNodesByLabel(const string& label) {
    // Check if the label exists in the index
    auto it = labelIndex.find(label);
    if (it != labelIndex.end()) {
        // Start JSON object
        cout << "{" << endl;
        cout << "  \"label\": \"" << label << "\"," << endl;
        cout << "  \"entities\": [" << endl;

        int count = 0;
        int totalNodes = it->second.size();

        for (auto node : it->second) {
            // Print each node's name
            cout << "    \"" << node->name << "\"";

            // Add a comma if this is not the last node
            if (++count < totalNodes) {
                cout << ",";
            }
            cout << endl;
        }

        cout << "  ]" << endl; // Close the entities array
        cout << "}" << endl;   // Close the JSON object
    } else {
        cout << "Error: Label \"" << label << "\" does not exist." << endl;
    }
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
                type->setProperty(key,value); // updating property of relationship
        }
        else
        {
            cout << "No relation found" << endl;
        }

    }

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
           
    void getRelation(const string& name1, const string& name2){
    
    //checking name1 or name2 exists or not.
          if(edges.find(name1)!=edges.end() && edges[name1].find(name2)!=edges[name2].end()){
                     
             return edges[name1][name2]->displayRelationship();
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

    void interpretQuery(const string& query){
       
       //check for ADD_ENTITY query
       if(query.find("ADD_ENTITY{")==0){
             // Locate the opening and closing braces
             int openBrace = query.find("{");
             int closeBrace = query.find("}");

             // Check for valid brace positions
            if (openBrace == string::npos || closeBrace == string::npos || closeBrace < openBrace) {
                   cout << "Error: Invalid query format." << endl;
                   return;
            }

             // Extract the content inside the braces
             string entityData = query.substr(openBrace + 1, closeBrace - openBrace - 1);
        
             // Split the entity data by comma
             int commaPos = entityData.find(",");
             if (commaPos == string::npos) {
                 cout << "Error: Missing comma between label and name." << endl;
                return;
              }

              // Extract label and name
              string label = entityData.substr(0, commaPos);
              string name = entityData.substr(commaPos + 1);

              // Trim any leading or trailing whitespace
              label.erase(0, label.find_first_not_of(" \n\r\t")); // Trim left
              label.erase(label.find_last_not_of(" \n\r\t") + 1); // Trim right
              name.erase(0, name.find_first_not_of(" \n\r\t")); // Trim left
              name.erase(name.find_last_not_of(" \n\r\t") + 1); // Trim right

              // Call the method to add the node
              addNode(label, name);
       }
        
       // Check for ADD_PROPERTY query
       else if (query.find("ADD_PROPERTY{") == 0) {
              int openBrace = query.find("{");
              int closeBrace = query.find("}");

              if (openBrace == string::npos || closeBrace == string::npos || closeBrace < openBrace) {
                 cout << "Error: Invalid ADD_PROPERTY query format." << endl;
                  return;
              }

              string propertyData = query.substr(openBrace + 1, closeBrace - openBrace - 1);
              int commaPos = propertyData.find(",");

              if (commaPos == string::npos) {
                  cout << "Error: Missing comma between name and properties." << endl;
                  return;
              }

              string name = propertyData.substr(0, commaPos);
              string properties = propertyData.substr(commaPos + 1);

              // Trim whitespace from name
              name.erase(0, name.find_first_not_of(" \n\r\t"));
              name.erase(name.find_last_not_of(" \n\r\t") + 1);

              // Split properties by comma and process each key-value pair
              stringstream ss(properties);
              string keyValuePair;
              while (getline(ss, keyValuePair, ',')) {
                  int colonPos = keyValuePair.find(":");
                  if (colonPos == string::npos) {
                      cout << "Error: Invalid property format for '" << keyValuePair << "'." << endl;
                      continue; // Skip this property and move to the next
                  }

                  string key = keyValuePair.substr(0, colonPos);
                  string value = keyValuePair.substr(colonPos + 1);

                  // Trim whitespace from key and value
                  key.erase(0, key.find_first_not_of(" \n\r\t"));
                  key.erase(key.find_last_not_of(" \n\r\t") + 1);
                  value.erase(0, value.find_first_not_of(" \n\r\t")); 
                  value.erase(value.find_last_not_of(" \n\r\t") + 1);

                  // Add the property to the node
                  addNodeProperty(name, key, value); // Assuming label is not needed here, or can be set to a default value
                  
             }
               
               return; // Exit the function after processing ADD_PROPERTY
       }

       //check for GET_INFO query
       else if (query.find("GET_INFO{") == 0) {
        int nameStart = query.find("{") + 1;
        int nameEnd = query.find(",", nameStart);

        if (nameEnd == string::npos) {
            cout << "Error: Malformed GET_INFO query - missing comma after name." << endl;
            return;
        }

        // Extract and trim the name
        string name = query.substr(nameStart, nameEnd - nameStart);
        name.erase(0, name.find_first_not_of(" \t\n\r"));  // Trim leading whitespace
        name.erase(name.find_last_not_of(" \t\n\r") + 1);  // Trim trailing whitespace

        // Extract and trim the keys string
        string keysStr = query.substr(nameEnd + 1, query.find("}") - nameEnd - 1);
        keysStr.erase(0, keysStr.find_first_not_of(" \t\n\r"));  // Trim leading whitespace
        keysStr.erase(keysStr.find_last_not_of(" \t\n\r") + 1);  // Trim trailing whitespace

        if (name.empty() || keysStr.empty()) {
            cout << "Error: Malformed GET_INFO query - name or keys missing." << endl;
            return;
        }

        vector<string> keys;
        if (keysStr == "ALL") {
            keys.push_back("ALL");
        } else {
            stringstream ss(keysStr);
            string key;
            while (getline(ss, key, ',')) {
                // Trim each key
                key.erase(0, key.find_first_not_of(" \t\n\r"));  // Trim leading whitespace
                key.erase(key.find_last_not_of(" \t\n\r") + 1);  // Trim trailing whitespace

                if (key.empty()) {
                    cout << "Error: Malformed GET_INFO query - empty key found." << endl;
                    return;
                }
                keys.push_back(key);
            }
        }

        // Retrieve properties
        getNodeProperty(name, keys);
    }
       
       //check for DELETE_INFO query
       else if (query.find("DELETE_INFO{") == 0) {
         // Parse DELETE_INFO query
         int nameStart = query.find("{") + 1;
         int nameEnd = query.find(",", nameStart);

         if (nameEnd == string::npos) {
             cout << "Error: Malformed DELETE_INFO query - missing comma after name." << endl;
             return;
         }

         string name = query.substr(nameStart, nameEnd - nameStart);
         string keysStr = query.substr(nameEnd + 1, query.find("}") - nameEnd - 1);
     
         if (name.empty() || keysStr.empty()) {
             cout << "Error: Malformed DELETE_INFO query - name or keys missing." << endl;
             return;
         }

         // Trim whitespace from name and keys
         name.erase(remove_if(name.begin(), name.end(), ::isspace), name.end());
         keysStr.erase(remove_if(keysStr.begin(), keysStr.end(), ::isspace), keysStr.end());

         vector<string> keys;

         if (keysStr == "ALL") {
             keys.push_back("ALL");
         } else {
             stringstream ss(keysStr);
          string key;
         while (getline(ss, key, ',')) {
            if (key.empty()) {
                cout << "Error: Malformed DELETE_INFO query - empty key found." << endl;
                return;
            }
            keys.push_back(key);
         }
       }

        // Call the deleteNodeProperty function with the name and keys
        deleteNodeProperty(name, keys);
      }
  
       else if (query.find("GET_LABELED{") == 0) {
            // Parse GET_LABELED query
            int labelStart = query.find("{") + 1; // Start after the opening brace
            int labelEnd = query.find("}", labelStart); // Find the closing brace

            // Check for malformed query
            if (labelEnd == string::npos) {
               cout << "Error: Malformed GET_LABELED query - missing closing brace." << endl;
               return;
           }
    
           // Extract the label
           string label = query.substr(labelStart, labelEnd - labelStart);
   
            // Trim whitespace
            label.erase(0, label.find_first_not_of(" \t\n\r")); // Trim leading whitespace
            label.erase(label.find_last_not_of(" \t\n\r") + 1); // Trim trailing whitespace

            // Check if the label is empty after trimming
            if (label.empty()) {
                cout << "Error: Malformed GET_LABELED query - label is missing." << endl;
                return;
            }

           // Retrieve nodes by label and print them in JSON format
            getNodesByLabel(label);
      }

        
       else {
        cout<<"Error: Unsupported query type."<<endl;
       }


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
