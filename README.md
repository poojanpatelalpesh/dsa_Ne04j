# In Memory Graph Database Similar to NEO4J # 

# Overview #

This project implements a query-based graph management system with nodes, relationships, and properties using a custom structure inspired by Neo4j. The program enables the creation, manipulation, and deletion of nodes and relationships using predefined query formats.

# Key Notes: #

1. Name as ID: Each node's name acts as its unique identifier, so no two nodes can share the same name.

2. Naming Format: Names should follow the format First_Last (e.g., John_Doe), with each word capitalized.

3. Relationships: Each pair of nodes can have only one relationship. Only single-sided relationships are considered.

# Components: #

1. Node Class:
   
   a. updateProperty: Known as addNodeProperty in the graph class; updates or adds properties to a node.

   b. getProperty: Known as getNodeProperty in the graph class; retrieves specific properties.

   c. printProperties: Known as getNodeProperty with ALL as the argument in the graph class; retrieves all properties.

   d. deleteProperty: Known as deleteNodeProperty in the graph class; deletes a specified property.

   e. clearProperties: Known as deleteNodeProperty with ALL as the argument in the graph class; deletes all properties.

3. Relationship Class:

   a. setProperty: Known as addRelationshipProperty in the graph class; adds or updates properties of a relationship.

   b. removeProperty: Known as deleteRelationshipProperty in the graph class; deletes a specific property of a relationship.

   c. clearProperties: Known as deleteRelationshipProperty with ALL as the argument in the graph class; deletes all properties of a relationship.

   d. getProperty: Known as getRelationshipProperty in the graph class; retrieves a specific property of a relationship.

   e. displayRelationship: Known as getRelationshipProperty with ALL as the argument in the graph class; retrieves all properties of a relationship.
   
5. Graph Class
The Graph class manages all nodes and relationships and includes methods for querying and manipulating the graph's structure.

# Queries and Functions #

1. Node Management:
   
   a. ADD_ENTITY{Label,Name}: Adds a node with the specified label and name.

   b. ADD_PROPERTY{Name,key1:value1,key2:value2...}: Adds or updates properties for a specified node.

   c. GET_INFO{Name,ALL}: Retrieves all properties of a specified node.

   d. GET_INFO{Name,Key1,Key2,...}: Retrieves specific properties of a node.

   e. DELETE_INFO{Name,ALL}: Deletes all properties of a specified node.

   f. DELETE_INFO{Name,Key1,Key2,...}: Deletes specified properties of a node.

   g. GET_LABELED{Label}: Retrieves nodes with a specified label.
   
3. Relationship Management:
   
   a. ADD_r{Name1,Name2,Relation}: Adds or updates a relationship between two nodes.

   b. ADD_r_PROPERTY{Name1,Name2,key1:value1,key2:value2...}: Adds or updates properties of a relationship.

   c. GET_r_INFO{Name1,Name2,ALL}: Retrieves all properties of a specified relationship.

   d. GET_r_INFO{Name1,Name2,Key1,Key2...}: Retrieves specific properties of a relationship.

   e. DELETE_r_INFO{Name1,Name2,ALL}: Deletes all properties of a relationship.

   f. DELETE_r_INFO{Name1,Name2,Key1,Key2...}: Deletes specified properties of a relationship.
   
5. Node Retrieval:

   a. FIND{Name,Relation1,Relation2...}: Finds related nodes based on specified relationships.

   b. FIND{Name,ALL}: Finds all nodes related to a specified node.

   c. DELETE_ENTITY{Label,Name}: Deletes a specified node.

   d. DELETE_r{Name1,Name2,Relation}: Deletes a specified relationship between two nodes.

   e. DELETE_r{Name1,Name2,ALL}: Deletes all relationships between two nodes.

   f. GET{key1:value1,key2:value2...}: Retrieves all nodes with the specified properties.

# Conclusion: #

This project offers a streamlined way to manage and interact with graph data using custom, query-based commands. With the ability to create nodes and relationships, add and retrieve properties, and perform targeted searches, this system is a powerful tool for simulating complex network relationships. By following the structured query format and naming conventions, users can explore diverse data scenarios effectively.

# Future Enhancements: #

This system provides a foundational structure for graph management but could be further extended in several ways:

1. Bidirectional Relationships: Currently, relationships are single-sided; adding support for bidirectional relationships could enhance versatility.

2. Enhanced Query Parsing: Implementing more complex query parsing with error handling could improve user experience.

3. Graph Visualization: Adding a visualization layer could help users see relationships more intuitively.

This project exemplifies how custom graph structures can be tailored to specific data management needs while being simple and user-friendly. Thank you for exploring this system!
   
