# Suffix Array

Builds a suffix array using the doubling algorithm.

## What it does

Returns positions of all suffixes of a string in lexicographical order.

## Idea

- First sort suffixes by 1 character
- Then iteratively double length: 2, 4, 8, ...
- Use equivalence classes instead of comparing strings directly

## Functions

- `FirstSort` — sorts suffixes by first character, builds initial classes  
- `Sort` — stable counting sort by current classes  
- `NewClasses` — rebuilds classes for doubled length  
- `SuffixArr` — main function that constructs the suffix array  

## Complexity

- Time: O(n log n)
- Memory: O(n)

## Notes

- Adds `'\0'` as smallest character
- Returns 0-based positions