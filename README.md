# Electrical Products Quality Tracking System

## Overview

This project implements a system to track the quality of electrical products using AVL trees in C. Each product is defined by its quality (`QUALITY`) and the time it was added to the system (`TIME`). The system supports various operations, including adding and removing products, querying by quality, and checking the existence of specific quality products.

## Data Structures

### Node
Represents a node in the AVL trees.

### DataStructure
Represents the entire data structure containing two AVL trees and other metadata.

## Functions

### InitDataStructure
Initializes an empty data structure with a special quality s.

### AddProduct
Adds a new product with given time and quality to the data structure.

### RemoveProduct
Removes the product added at the specified time from the data structure.

### RemoveQuality
Removes all products with the specified quality from the data structure.

### GetIth
Returns the time of the product with the special quality s. If no such product exists, returns -1.

### GetIthRankBetween
Returns the time of the i-th quality product added between time1 and time2 inclusive. If no such product exists, returns -1.

### Exists
Returns 1 if there is a product with the special quality s, otherwise returns 0.

## Contributors
Omer Garabagi
Michel Rempel
