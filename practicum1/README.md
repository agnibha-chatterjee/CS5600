# Team Memebers
1. Tarun M - m.t@northeastern.edu
2. Agnibha Chatterjee - chatterjee.ag@northeastern.edu

# Hierarchical Memory and Cache System

This project implements a message-oriented data store with a memory hierarchy and caching system that efficiently manages messages between main memory and disk storage.

## Components

### Message System

- Defined in `message.h` and `message.c`
- Message structure includes:
  - Unique ID (UUID format)
  - Timestamp
  - Sender and receiver information
  - Message content
  - Delivery status flag
- Core functions:
  - `create_msg()`: Creates new messages with properly initialized fields
  - `store_msg()`: Persists messages to disk in the message_store directory
  - `retrieve_msg()`: Finds messages by UUID, checking cache first

### Cache System

- Defined in `cache.h` and `cache.c`
- Provides in-memory storage for frequently accessed messages
- Features:
  - Configurable cache size (default: 2 entries)
  - Two page replacement algorithms:
    - LRU (Least Recently Used)
    - Random replacement
  - Metrics tracking (hits, misses, hit ratio)

### Evaluation Framework

- Implemented in `cache_evaluation.c`
- Tests cache performance with 1000 random message accesses
- Measures and reports:
  - Cache hits per 1000 accesses
  - Cache misses per 1000 accesses
  - Cache hit ratio
- Evaluates both LRU and Random replacement strategies

## Building and Running

```
make          # Build all components
Usage:
./app create <sender> <receiver> <message_content> [--strategy=lru|random] - Creates a new message with an auto-generated UUID and stores it.
./app store <uuid> <sender> <receiver> <message_content> [--strategy=lru|random] - Creates a new message with the specified UUID and stores it.
./app retrieve <uuid> [--strategy=lru|random] - Retrieves and displays the message with the given UUID.
./app delete all [--strategy=lru|random] - Clears the contents of the message_store folder.
./app cache-stats - Displays cache hit/miss statistics.

Options:
--strategy=lru Use LRU (Least Recently Used) cache replacement strategy
--strategy=random Use Random cache replacement strategy

./cache_eval  # Run the cache evaluation program
```

## Design Considerations

The project implements a hierarchical memory system where:

1. Messages are stored in fixed-size cache slots in main memory
2. When the cache is full, messages are replaced according to the selected strategy
3. All messages are persisted to disk, organized by UUID
4. When retrieving messages, the cache is checked before accessing disk storage# Cache Data Structures and Strategy

This document outlines the data structures implemented to achieve efficient message lookup within the caching mechanism.

### Doubly Linked List (LRU Tracking)

**Purpose:**

- Maintains the order of message accesses, placing recently accessed messages at the front and least recently accessed at the back.

**Key Operations:**

- **Insertion:** `O(1)` (head insertion)
- **Removal:** `O(1)` (from tail or mid-list upon access)

**Reasons for Selection:**

- Constant-time complexity (`O(1)`) for insertion, removal, and updating of access order.
- Efficient management and quick eviction of least recently used messages when the cache is full.

### Hash Map (Key-Value Storage)

**Purpose:**

- Provides rapid message retrieval by UUID.

**Key Operations:**

- **Lookup:** `O(1)`
- **Insertion:** `O(1)`
- **Deletion:** `O(1)`

**Reasons for Selection:**

- Offers immediate and constant-time access to cached messages using unique identifiers.
- Essential for high-performance retrieval scenarios.

### Random Replacement Strategy

**Implementation:**

- Selects a cache entry for eviction using random indexing, safely computed with `rand()` function modulo the cache size.

**Purpose:**

- Provides an alternative caching strategy for performance comparison with the LRU approach.

## Alternative Approaches Considered

### Array-Based Cache

**Issues:**

- Inefficient (`O(n)`) operations for lookup, insertion, and removal due to required element shifting.
- Limited scalability with growing cache sizes.

### Singly Linked List

**Issues:**

- Inefficient (`O(n)`) removal, requiring traversal to predecessor nodes.
- Poor performance for efficient LRU management.

### Binary Search Tree (BST)

**Issues:**

- Average complexity of `O(log n)` for lookup, insertion, and deletion operations.
- Additional complexity arising from the need to maintain tree balance (AVL or Red-Black trees).

## Rationale for Final Choice

Combining a hash map with a doubly linked list delivers optimal performance and simplicity:

- Ensures constant-time complexity for essential cache operations.
- Efficiently maintains message ordering and implements the LRU algorithm.
- Scalable and suitable for high-performance caching scenarios.

## Test Analysis

### Initialized with 16 slots
### CACHE WITH LRU STRATEGY
CACHE EVALUATION RESULTS (LRU):
- Number of cache hits per 1000 accesses: 446
- Number of cache misses per 1000 accesses: 554
- Cache hit ratio: 0.45

### CACHE WITH RANDOM STRATEGY
CACHE EVALUATION RESULTS (Random):
- Number of cache hits per 1000 accesses: 439
- Number of cache misses per 1000 accesses: 561
- Cache hit ratio: 0.44





