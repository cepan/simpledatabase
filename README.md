A small version of a relational database management system (DBMS), as well as a DSL (Domain Specific Language) with which a user can interact with the database.
The language will be implementing is similar to the declarative programming language SQL, which is used in the real world to interact with and query databases,
large or small.

libaries are designed top- to bottom

Purpose:
Program implements a database that responds to sql-like commands:

Grammar:

CREATE | MAKE : {  <create | make> table <TABLE_NAME> fields <FIELD_NAME> [, <FIELD_NAME>...]}

INSERT: { insert <INTO> <TABLE_NAME> values <VALUE> [, <VALUE>...]}

SELECT: {  

          select <* | FIELD_NAME> [, ,FIELD_NAME>...]					
          
          from <TABLE_NAME>         
          
          where <FIELD_NAME> <RELATIONAL_OPERATOR> <VALUE>     
          
          [<LOGICAL_OPERATOR>   
          
          <FIELD_NAME> <RELATIONAL_OPERATOR> <VALUE>...]
          
}

VALUE: A string of alphanumeric characters, or a string of alphanumeric
 			characters and spaces enclosed by double quotation marks:
 			"Jean Luise", Finch, 1923
      
RELATIONAL OPERATOR: [ = | > | < | >= | <= ]

LOGICAL OPERATOR: [and | or]

Data Structure:

 

Program uses Maps and Multimaps based on B+Trees as the primary data structure:

Individual indices, list of indices, list of keywords etc.

Program uses binary files for disk storage of database record data

Program uses text files for storing names of fields, tables, number of records and other information that needs to be remembered from session to session.

Program uses a state machine (an adjacency matrix implementation of a graph) representing the grammar for command line interpretation.
