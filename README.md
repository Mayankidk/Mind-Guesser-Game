# Mind Guesser (Akinator in C++)

A self-learning "Twenty Questions" decision engine implemented in modern C++ using a dynamic, self-growing binary tree.

---

## 📌 Overview
Mind Guesser is an intelligent guessing engine inspired by the classic **20 Questions** game and the web application **Akinator**. 

Unlike conventional programs where rules and logic are hardcoded, Mind Guesser decouples the game engine from domain knowledge. The binary decision tree **is** the intelligence:
* **Internal Nodes:** Represent Yes/No discriminative questions.
* **Leaf Nodes:** Represent entity guesses (answers).
* **Self-Learning ($\mathcal{O}(1)$):** When the engine fails to guess the player's secret entity, it prompts for the correct item and a distinguishing question, dynamically grafting a new question node into the tree in real time.
* **Preorder Serialization:** State is saved to plain text files using recursive preorder traversal, allowing complete knowledge persistence across sessions.
* **Fault-Tolerant Atomic Saves:** Writes to temporary `.tmp` buffers before atomically renaming to prevent file corruption.

---

## 👥 Authors & Academic Context
* **Authors:** Mayank Singh & Vishvam Adeshara
* **Course:** 6CS102CC26 – Data Structures
* **Faculty Guide:** Prof. Nirav Patel
* **Institution:** Department of Computer Science & Engineering, Institute of Technology, Nirma University

---

## 🚀 Quick Start

### 1. Build
Compile using `g++` with all standard warnings enabled:
```bash
g++ -Wall -Wextra akinator.cpp -o akinator.exe
```

### 2. Run
Run the executable directly, or pass a topic file as a command-line argument:
```bash
# Interactive mode (prompts for topic file):
./akinator.exe

# Or pass a topic file directly:
./akinator.exe animals.txt
```

Upon loading, the engine gives you the option to view the tree hierarchy and node statistics before starting the game:
```text
Topic file name (e.g. chess.txt): animals.txt
Do you want to view the tree hierarchy? (y/n): y
```

---

## 📁 Repository Structure
```text
.
├── akinator.cpp               # Core C++ decision tree engine
├── animals.txt                # 30-animal dataset (depth 8, 29 questions)
├── animals_condensed.txt      # 4-animal dataset matching report walkthrough
├── chess_players.txt          # 4-player dataset (Carlsen, Kasparov, Caruana, Nakamura)
├── chess.txt                  # 6-piece chess ruleset dataset
├── programming_languages.txt  # 10-language computer science dataset
├── .gitignore                 # Ignores compiled binaries (*.exe) and temp files
└── README.md                  # Project documentation
```

---

## 🧠 Algorithmic Properties

| Metric | Complexity | Notes |
| :--- | :---: | :--- |
| **Gameplay Traversal** | $\mathcal{O}(d)$ | Descends one branch per question (depth $d$) |
| **Dynamic Learning Step** | $\mathcal{O}(1)$ | Allocates 2 nodes and rewires 3 pointers |
| **Preorder Serialization** | $\mathcal{O}(n)$ | Single-pass traversal of all $2n - 1$ nodes |
| **Reconstruction (Load)** | $\mathcal{O}(n)$ | Deterministic single-pass file parsing |
| **Memory Footprint** | $\mathcal{O}(n)$ | Exactly $n$ leaves + $(n - 1)$ questions |

### Full Binary Tree Theorem
The tree strictly enforces the full binary tree property:
$$\text{Leaves } (L) = \text{Internal Questions } (I) + 1 \implies \text{Total Nodes } = 2I + 1$$
Every learning cycle converts 1 leaf into 1 question and 2 leaves ($\Delta I = +1, \Delta L = +1$), preserving the invariant indefinitely.

---

## 📜 License
Educational project developed for MCA Semester 1 Data Structures curriculum at Nirma University.
