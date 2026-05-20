# MiniRedeSocial

A mini social network implemented in C++ as a final project for the Data Structures course at UFRGS. The system processes text commands to manage users, followers, posts, likes, notifications, and feeds.

## How it works

The program reads commands from stdin and writes results to stdout. Each command triggers a specific operation on the social network's internal state.

```sh
# Compile
g++ minirede.cpp -o minirede

# Run with a test file
./minirede < testes/pequeno/entrada.txt
```

## Supported commands

| Command | Description |
|---|---|
| `ADD_USER <id> <username> <name>` | Register a new user |
| `FIND_USER <id>` | Look up a user by numeric ID |
| `FIND_USERNAME <username>` | Look up a user by username |
| `LIST_USERS` | List all users sorted by ID |
| `FOLLOW <followerId> <followedId>` | Make one user follow another |
| `LIST_FOLLOWING <id>` | List who a user follows |
| `ADD_POST <postId> <authorId> <timestamp> <text>` | Create a post |
| `LIKE <userId> <postId>` | Like a post |
| `GET_NOTIFICATIONS <userId> <k>` | Get the k most recent notifications |
| `FEED <userId> <k>` | Get the k most recent posts from followed users |
| `TOP_POSTS <k>` | Get the k most liked posts |
| `END` | Stop processing |

## Data structure decisions

### BST for user lookup by ID — O(log n)

Users are stored in a **Binary Search Tree (BST)** keyed on the numeric user ID. This means:

- `FIND_USER` navigates the tree in O(log n) comparisons instead of scanning a list in O(n).
- `LIST_USERS` performs an **in-order traversal**, which visits nodes in ascending ID order automatically — no sorting step needed.
- Insertion also runs in O(log n) and naturally rejects duplicates when it finds an existing node with the same ID.

A plain array or linked list would require O(n) scans for every lookup, which degrades quickly as the user base grows.

### Hash table for username lookup — O(1) average

Finding a user by username is an entirely different access pattern: the BST is keyed on ID, so searching by string would require a full O(n) traversal.

To solve this, every user is also indexed in a **hash table** keyed on the username string. A polynomial rolling hash (`key = key * 31 + c`) maps each username to a bucket. Collisions are resolved with chaining (linked list per bucket).

Critically, **the hash table stores a pointer to the same `Usuario` object** allocated by the BST — there is no duplication of user data. Both indexes are always in sync and free is handled once through the BST.

### Sorted linked list for followers — deduplication in O(n)

Each user's follower list is a **linked list kept in ascending ID order**. Insertion walks the list once and aborts if it finds the ID already present, guaranteeing no duplicate follows without needing a separate existence check. The sorted order also means the `LIST_FOLLOWING` output is always deterministic.

## Project structure

```
minirede.h      — struct definitions and function declarations
minirede.cpp    — full implementation + main()
testes/         — public test cases with expected output
```
