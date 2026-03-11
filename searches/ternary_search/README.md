# Ternary Search

Simple ternary search to find an approximate minimum of a unimodal function on a real interval `[left, right]`.

The function repeatedly shrinks the search range until it becomes smaller than `eps`, then returns the result.