# Aho-Corasick

A simple implementation of the Aho-Corasick algorithm for multiple pattern matching on lowercase strings (`'a'..'z'`).

## API

- `AddString(s)` — inserts a pattern into the trie
- `CheckString(s)` — checks whether a string exists in the trie
- `Aho_Corasick(text, patterns)` — finds all occurrences of all patterns in the text

## Idea

The algorithm builds a trie of patterns, then adds suffix links and transitions to process the text in linear time.

## Complexity

- Trie build: O(sum of pattern lengths)
- Text processing: O(|text| + total matches)
- Memory: O(sum of pattern lengths * alphabet)