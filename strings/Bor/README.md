# Trie (Bor)

Simple trie implementation for lowercase strings (`'a'..'z'`).

## API

- `AddString(s)` — inserts a string, returns the last node index
- `CheckString(s)` — returns `true` if the string exists in the trie

## Idea

Each node stores transitions to children and a `terminal` flag indicating the end of a word.

## Complexity

- Insert: O(|s|)
- Lookup: O(|s|)