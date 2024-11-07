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

    // Stores relationships between nodes, keyed by "from node name" and "to node name".
    unordered_map<string, unordered_map<string, Relationship*>> relationships;

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

    void addRelationship(const string& nodeName1, const string& nodeName2, const string& relationshipType) {
    // Check if both nodes exist in the graph
    if (nodes.find(nodeName1) == nodes.end() || nodes.find(nodeName2) == nodes.end()) {
        cout << "Error: One or both nodes not found in the graph." << endl;
        return; // Exit the function if either node does not exist
    }

    // Check if the relationship already exists
    auto it = relationships[nodeName1].find(nodeName2);
    if (it != relationships[nodeName1].end()) {
        // Update the existing relationship type
        it->second->relation = relationshipType; // Update the relationship type
        cout << "Updated relationship between " << nodeName1 << " and " << nodeName2 << " to " << relationshipType << "." << endl;
    } else {
        // Create a new relationship
        Relationship* newRelationship = new Relationship(relationshipType);

        // Store the relationship in the relationships map
        relationships[nodeName1][nodeName2] = newRelationship;
        cout << "Added relationship between " << nodeName1 << " and " << nodeName2 << " as " << relationshipType << "." << endl;
    }
}

    void addRelationshipProperty(const string& name1, const string& name2, const string& key, const string& value) {
    // Check if the nodes and relationship exist in the graph
    if (relationships.find(name1) == relationships.end()) {
        cout << "Error: No relationships found for node \"" << name1 << "\"." << endl;
        return;
    }
    
    if (relationships[name1].find(name2) == relationships[name1].end()) {
        cout << "Error: No relationship exists between \"" << name1 << "\" and \"" << name2 << "\"." << endl;
        return;
    }

    // Retrieve the existing relationship between name1 and name2
    Relationship* relationship = relationships[name1][name2];
    if (!relationship) {
        cout << "Error: Relationship object is null." << endl;
        return;
    }

    // Set or update the specified property of the relationship
    relationship->setProperty(key, value);
    cout << "Property \"" << key << "\" set to \"" << value << "\" for relationship between \"" 
         << name1 << "\" and \"" << name2 << "\"." << endl;
}

    void getRelationshipProperty(const string& name1, const string& name2, const vector<string>& keys) { 
    // Check if there are relationships for name1
    if (relationships.find(name1) == relationships.end()) {
        cout << "Error: No relationships found for node \"" << name1 << "\"." << endl;
        return;
    }

    // Check if there is a specific relationship from name1 to name2
    auto& relatedNodes = relationships[name1];
    if (relatedNodes.find(name2) == relatedNodes.end()) {
        cout << "Error: No relationship exists between \"" << name1 << "\" and \"" << name2 << "\"." << endl;
        return;
    }

    // Retrieve the relationship
    Relationship* relationship = relatedNodes[name2];
    if (!relationship) {
        cout << "Error: Relationship object is null." << endl;
        return;
    }

    // If "ALL" is the only key, display all properties
    if (keys.size() == 1 && keys[0] == "ALL") {
        relationship->displayRelationship();
    } else {
        // Output properties in JSON-like format
        cout << "{\n  \"Relationship\": \"" << relationship->relation << "\",\n  \"Properties\": {\n";
        bool first = true; // Flag to manage commas between properties
        for (const string& key : keys) {
            string value = relationship->getProperty(key);
            if (!value.empty()) {
                if (!first) {
                    cout << ",\n"; // Add comma for all but the first element
                }
                cout << "    \"" << key << "\": \"" << value << "\"";
                first = false;
            } else {
                cout << "    // Property \"" << key << "\" not found\n";
            }
        }
        cout << "\n  }\n}\n";
    }
}

    void RetrieveRelatedNodes(const string& name,const string& relation){       //[what if we want to retrive all connected node irrespective of relationship?];
       if(relationships.find(name)!=relationships.end()){
        //iterating over nodes that are connected to given name.
         cout<<"related Nodes to " <<name<<" are : ";
            for(const auto& it: relationships[name]){
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
           
    void deleteNode(const string label, const string name){
             if(nodes.find(name)!=nodes.end()){
                // 1. Remove all outgoing relationships from this node
                  relationships.erase(name);

               // 2. Remove all incoming relationships to this node
                for (auto &it : relationships) {
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
          if(relationships.find(name1)!=relationships.end() && relationships[name1].find(name2)!=relationships[name2].end()){
            //getting relationship
             Relationship* rel = relationships[name1][name2];
             //checking if relation type match or not.
             if(rel->relation==relationtype){ 
                 //first clearing all properties a relation have
                 rel->properties.clear();

                 //now deleting relation itself
                 delete rel;
                 
                 relationships[name1].erase(name2);

                 //[what if name1 only have one relation with name2?]
                 if (relationships[name1].empty()) {
                         relationships.erase(name1);
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

        // Extract and trim the `name`
        string name = query.substr(nameStart, nameEnd - nameStart);
        name.erase(0, name.find_first_not_of(" \t\n\r"));  // Trim leading whitespace
        name.erase(name.find_last_not_of(" \t\n\r") + 1);  // Trim trailing whitespace

        // Extract and trim the `keys` string
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
       
       //check for GET_LABELED query
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
      
       //check for ADD_r query
       else if (query.find("ADD_r{") == 0) {
             // Find the start and end of the curly braces
             int startPos = query.find("{") + 1; // Position just after '{'
             int endPos = query.find("}");
    
             // Check if curly braces are correctly placed
             if (endPos == string::npos) {
                 cout << "Error: Malformed ADD_r query - missing closing brace." << endl;
                 return;
             }
    
             // Extract the content between the braces
             string content = query.substr(startPos, endPos - startPos);
    
             // Split the content by commas
            stringstream ss(content);
            string name1, name2, relation;

            // Extract Name1
            getline(ss, name1, ',');
             // Extract Name2
             getline(ss, name2, ',');
             // Extract Relation
             getline(ss, relation, ',');

              // Check for empty values and trim whitespace
              if (name1.empty() || name2.empty() || relation.empty()) {
                  cout << "Error: Malformed ADD_r query - one or more parameters are missing." << endl;
                  return;
             }

             // Trim whitespace (if necessary)
             name1.erase(0, name1.find_first_not_of(" \t\n\r")); // Trim leading whitespace
             name1.erase(name1.find_last_not_of(" \t\n\r") + 1); // Trim trailing whitespace
             name2.erase(0, name2.find_first_not_of(" \t\n\r")); // Trim leading whitespace
             name2.erase(name2.find_last_not_of(" \t\n\r") + 1); // Trim trailing whitespace
             relation.erase(0, relation.find_first_not_of(" \t\n\r")); // Trim leading whitespace
             relation.erase(relation.find_last_not_of(" \t\n\r") + 1); // Trim trailing whitespace
 
             // Call the function to add the relationship
            addRelationship(name1, name2, relation);
      }
 
       //check for ADD_r_PROPERTY query
       else if (query.find("ADD_r_PROPERTY{") == 0) {
            // Extracting the main part of the query
            int nameStart = query.find("{") + 1;
            int nameEnd = query.find(",", nameStart);
        
            if (nameEnd == string::npos) {
                cout << "Error: Malformed ADD_r_PROPERTY query - missing comma after Name1." << endl;
                return;
            }

            string name1 = query.substr(nameStart, nameEnd - nameStart);
            int name2Start = nameEnd + 1;
            int name2End = query.find(",", name2Start);

            if (name2End == string::npos) {
                        cout << "Error: Malformed ADD_r_PROPERTY query - missing comma after Name2." << endl;
               return;
            }

             string name2 = query.substr(name2Start, name2End - name2Start);
             string propertiesStr = query.substr(name2End + 1, query.find("}") - name2End - 1);

             if (name1.empty() || name2.empty() || propertiesStr.empty()) {
                 cout << "Error: Malformed ADD_r_PROPERTY query - Name1, Name2, or properties missing." << endl;
                 return;
             }
         
             // Parsing key-value pairs
             stringstream ss(propertiesStr);
             string pair;
          while (getline(ss, pair, ',')) {
                 int colonPos = pair.find(':');
                 if (colonPos == string::npos) {
                     cout << "Error: Malformed property pair \"" << pair << "\" - missing colon." << endl;
                     return;
                 }

              string key = pair.substr(0, colonPos);
              string value = pair.substr(colonPos + 1);

              // Remove extra whitespace from key and value
              key.erase(0, key.find_first_not_of(" \t\n\r"));
              key.erase(key.find_last_not_of(" \t\n\r") + 1);
              value.erase(0, value.find_first_not_of(" \t\n\r"));
              value.erase(value.find_last_not_of(" \t\n\r") + 1);

              if (key.empty() || value.empty()) {
                  cout << "Error: Empty key or value in property pair \"" << pair << "\"." << endl;
               return;
               }
  
              // Add or update the relationship property
              addRelationshipProperty(name1, name2, key, value);
          }
      }
       
       //check for GET_r_INFO query
       else if (query.find("GET_r_INFO{") == 0) {
             // Parse the query to extract Name1, Name2, and keys
             int start = query.find("{") + 1;
             int end = query.find("}", start);
             if (end == string::npos) {
                 cout << "Error: Malformed GET_r_INFO query - missing closing '}'." << endl;
                 return;
             }

             string content = query.substr(start, end - start);

             // Find the first comma, which separates Name1 and Name2
             int commaPos1 = content.find(",");
             if (commaPos1 == string::npos) {
                 cout << "Error: Malformed GET_r_INFO query - missing first comma." << endl;
                 return;
             }

              string name1 = content.substr(0, commaPos1);
              name1.erase(0, name1.find_first_not_of(" \t\n\r")); // Trim leading whitespace
              name1.erase(name1.find_last_not_of(" \t\n\r") + 1); // Trim trailing whitespace

              // Find the second comma, which separates Name2 and the keys or "ALL"
              int commaPos2 = content.find(",", commaPos1 + 1);
              if (commaPos2 == string::npos) {
                  cout << "Error: Malformed GET_r_INFO query - missing second comma." << endl;
                  return;
              }

              string name2 = content.substr(commaPos1 + 1, commaPos2 - commaPos1 - 1);
              name2.erase(0, name2.find_first_not_of(" \t\n\r")); // Trim leading whitespace
              name2.erase(name2.find_last_not_of(" \t\n\r") + 1); // Trim trailing whitespace

               // Extract keys or "ALL"
              string keysStr = content.substr(commaPos2 + 1);
              keysStr.erase(0, keysStr.find_first_not_of(" \t\n\r")); // Trim leading whitespace
              keysStr.erase(keysStr.find_last_not_of(" \t\n\r") + 1); // Trim trailing whitespace

                 vector<string> keys;
                 if (keysStr == "ALL") {
                     keys.push_back("ALL");
             } else {
                     stringstream ss(keysStr);
                  string key;
            while (getline(ss, key, ',')) {
             key.erase(0, key.find_first_not_of(" \t\n\r")); // Trim leading whitespace
             key.erase(key.find_last_not_of(" \t\n\r") + 1); // Trim trailing whitespace
             if (!key.empty()) {
                keys.push_back(key);
             } else {
                cout << "Error: Malformed GET_r_INFO query - empty key found." << endl;
                return;
            }
           }
           }

             // Retrieve relationship properties
               getRelationshipProperty(name1, name2, keys);
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
