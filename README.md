Graph Database Implementation:

This project is a simple graph database implementation inspired by Neo4j, written in C++. It supports essential CRUD operations and basic graph traversal queries. The database allows users to create, read, update, and delete nodes (entities) and relationships (edges) between them.

Features
1. Node Management: Create, update, and delete nodes with customizable properties.
2. Relationship Management: Establish and manage relationships between nodes, including custom properties for relationships.
3. Label Indexing: Fast querying of nodes based on labels.
4. Query Language: Basic query support similar to Cypher, including CREATE and MATCH commands.
5. Property Management: Add or update properties for both nodes and relationships.

Classes:

Node:
Represents an entity in the graph, holding:
1. Label: A type for the entity (e.g., Person, Product).
2. Name: A unique identifier for the node.
3. Properties: Key-value pairs to store additional information about the node.

Relationship:
Represents a connection between two nodes, containing:
1. Relation Type: The type of relationship (e.g., FRIENDS, PURCHASED).
2. Properties: Key-value pairs to store additional information about the relationship.

Graph:
Manages the overall structure of the database, including:
1. Node Storage: A map to hold all nodes.
2. Label Index: An index for fast lookups by label.
3. Relationship Storage: A map of maps to manage relationships between nodes.

Usage:

Building and Running:
To compile and run the code:

1. Clone the repository:

*bash*

git clone https://github.com/yourusername/graph-database.git
cd graph-database

2. Compile the code:

*bash*

g++ -o graph_database main.cpp

3. Run the executable:

*bash*

./graph_database

Example Commands:

1. Create a Node:

*css*

CREATE (n:Person {name: 'Alice'})

2. Set Node Property:

*php*

CREATE (n:Person {name: 'Alice'}) SET n.age="30"

3. Match Nodes:

*css*

MATCH (n:Person {name: 'Alice'})

Query Syntax:

CREATE: Add new nodes or set properties.

MATCH: Retrieve nodes based on label and properties.

Limitations:
1. The current implementation does not handle concurrent modifications or transactions.
2. There is no support for complex queries beyond basic matching and property retrieval.
3. Memory management for dynamic allocations can be improved with smart pointers.

Contributing:
Contributions are welcome! If you find a bug or have suggestions for improvements, please open an issue or submit a pull request.
