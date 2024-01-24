Project for the course Algoritmi e Principi dell'Informatica (API) @ Politecnico di Milano 
A.A. 2021-2022 

# README

## Educational Objectives and Implementation

### Objectives
- Practical application of the techniques learned in the module of algorithms and data structures of the course on algorithms and principles of computer science.
- Implementation of a solution to a problem, focusing on concrete aspects of code efficiency.

### Implementation
- Language: C (C11, VLA allowed)
- No external libraries beyond the standard C library.
- No multithreading.
- Input data received via stdin; results to be output via stdout.

### Implementation Mode
- The project is strictly individual.
- Do not use any code fragments from others.
- You are responsible for your own code; do not upload it to public repositories.
- Do not share it with colleagues for "inspiration" or use any found code fragments.
- In case of plagiarism or use of someone else's code, all involved projects will be canceled.

### Evaluation Criteria
- The correctness and efficiency of the proposed solution are evaluated with automated test batteries.
- Example input/output will be provided for local testing.
- Do not submit solutions without verifying they work locally.
- A tool for automatic generation of test cases (input/output) will be provided to facilitate local testing.
- The verification system calculates machine time and memory used.
- Evaluation is immediately calculated (and visible) through 3 test batteries:
  - The first worth 18 points (pass or fail).
  - The second up to 12 (6 tests worth 2 points each).
  - The last for distinction.

### Evaluation Criteria
- No limit to the number of submissions, nor penalties for multiple submissions.
- You can improve your evaluation as many times as you wish.
- Note: The last submission made to each test battery is evaluated. All evaluated submissions must use the same source.
- If in doubt, resubmit the same source to all test batteries as a precaution.
- Verifier available at https://dum-e X deib X polimi X it, substitute X with dot
- Access credentials via Polimi email.

### GraphRanker
- This year's project goal is the management of a ranking among weighted directed graphs.
- The ranking keeps track of the "best" k graphs.
- The program to be implemented receives
  - Two parameters, once only (on the first line of the file, separated by space)
    - d: the number of nodes of the graphs.
    - k: the length of the ranking.
  - A sequence of commands among
    - AddGraph [adjacency-matrix]
    - TopK

#### AddGraph
Requires adding a graph to those considered for ranking. It is followed by the adjacency matrix of the graph itself, printed one line per row, with elements separated by commas.
The nodes of the graph are logically labeled with an integer index from 0 to d-1; the node at position 0 is the one whose outgoing star is described by the first line of the matrix.
The weights of the graph's edge elements are integers in the range [0, 2^32 – 1].

Example for d=3:

```
AddGraph
3,7,42
0,7,2
7,4,3
```
