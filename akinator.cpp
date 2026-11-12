#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
using namespace std;

struct Node
{
    string text;
    Node *yes;
    Node *no;

    Node(const string &t) : text(t), yes(nullptr), no(nullptr) {}
    ~Node()
    {
        delete yes;
        delete no;
    }
    bool isLeaf() const { return yes == nullptr && no == nullptr; }
};

bool askYesNo(const string &prompt)
{
    while (true)
    {
        cout << prompt << " (y/n): ";
        string s;
        if (!getline(cin, s))
            exit(0);
        if (!s.empty())
        {
            char ch = tolower(s[0]);
            if (ch == 'y')
                return true;
            if (ch == 'n')
                return false;
        }
        cout << "Please type y or n.\n";
    }
}

string askLine(const string &prompt)
{
    string s;
    while (s.empty())
    {
        cout << prompt;
        if (!getline(cin, s))
            exit(0);
    }
    return s;
}

void playRound(Node *&root)
{
    Node *parent = nullptr;
    bool cameFromYes = false;
    Node *cur = root;

    while (!cur->isLeaf())
    {
        bool ans = askYesNo(cur->text);
        parent = cur;
        cameFromYes = ans;
        cur = ans ? cur->yes : cur->no;
    }

    if (askYesNo("Is it " + cur->text + "?"))
    {
        cout << "I got it!\n";
        return;
    }

    string correct = askLine("I give up. What were you thinking of? ");
    string q = askLine("Type a question that is YES for " + correct +
                       " and NO for " + cur->text + ": ");

    Node *newQ = new Node(q);
    newQ->yes = new Node(correct);
    newQ->no = cur;

    if (parent == nullptr)
        root = newQ;
    else if (cameFromYes)
        parent->yes = newQ;
    else
        parent->no = newQ;
    cout << "Thanks, I'll remember that.\n";
}

void save(Node *n, ostream &out)
{
    if (n->isLeaf())
    {
        out << "A:" << n->text << "\n";
        return;
    }
    out << "Q:" << n->text << "\n";
    save(n->yes, out);
    save(n->no, out);
}

Node *load(istream &in)
{
    string line;
    if (!getline(in, line) || line.size() < 2 || line[1] != ':')
        return nullptr;

    string text = line.substr(2);
    if (line[0] == 'A')
        return new Node(text);

    if (line[0] == 'Q')
    {
        Node *n = new Node(text);
        n->yes = load(in);
        n->no = load(in);
        if (!n->yes || !n->no)
        {
            delete n;
            return nullptr;
        }
        return n;
    }
    return nullptr;
}

void saveFile(const string &path, const string &topic, Node *root)
{
    string tempPath = path + ".tmp";
    {
        ofstream out(tempPath);
        if (!out)
        {
            cerr << "Warning: Could not open temporary file for saving.\n";
            return;
        }
        out << "TOPIC:" << topic << "\n";
        save(root, out);
    }
    // Atomic replacement: remove target and rename temporary file
    remove(path.c_str());
    if (rename(tempPath.c_str(), path.c_str()) != 0)
    {
        cerr << "Warning: Could not update " << path << "\n";
    }
}

Node *loadFile(const string &path, string &topic)
{
    ifstream in(path);
    if (!in)
        return nullptr;

    string first;
    if (!getline(in, first))
        return nullptr;

    if (first.rfind("TOPIC:", 0) == 0)
        topic = first.substr(6);
    else
    {
        topic = "something";
        in.clear();
        in.seekg(0);
    }
    return load(in);
}

Node *createNewTopic(string &topic)
{
    topic = askLine("What is the topic? (e.g. chess players, animals, openings): ");
    string a = askLine("Name one thing from this topic: ");
    string b = askLine("Name a different one: ");
    string q = askLine("Type a question that is YES for " + a + " and NO for " + b + ": ");

    Node *root = new Node(q);
    root->yes = new Node(a);
    root->no = new Node(b);
    return root;
}

int countQuestions(const Node *n)
{
    if (!n || n->isLeaf())
        return 0;
    return 1 + countQuestions(n->yes) + countQuestions(n->no);
}

int countLeaves(const Node *n)
{
    if (!n)
        return 0;
    if (n->isLeaf())
        return 1;
    return countLeaves(n->yes) + countLeaves(n->no);
}

int treeHeight(const Node *n)
{
    if (!n)
        return 0;
    return 1 + max(treeHeight(n->yes), treeHeight(n->no));
}

void printTree(const Node *n, const string &prefix = "", bool isYes = true, bool isRoot = true)
{
    if (!n)
        return;
    cout << prefix;
    if (isRoot)
        cout << "[ROOT] ";
    else
        cout << (isYes ? "+-- [YES] " : "\\-- [NO]  ");

    if (n->isLeaf())
    {
        cout << "Answer: " << n->text << "\n";
    }
    else
    {
        cout << "Question: " << n->text << "\n";
        string childPrefix = prefix + (isRoot ? "  " : (isYes ? "|     " : "      "));
        printTree(n->yes, childPrefix, true, false);
        printTree(n->no, childPrefix, false, false);
    }
}

void viewTreeStats(const string &topic, const string &path, Node *root)
{
    cout << "\n========================================\n";
    cout << " TOPIC: " << topic << " (" << path << ")\n";
    cout << "========================================\n";
    cout << " Questions (Internal Nodes): " << countQuestions(root) << "\n";
    cout << " Answers   (Leaf Nodes):     " << countLeaves(root) << "\n";
    cout << " Total Nodes:                " << (countQuestions(root) + countLeaves(root)) << "\n";
    cout << " Maximum Tree Depth:         " << treeHeight(root) << "\n";
    cout << "----------------------------------------\n";
    cout << " Visual Hierarchy:\n";
    cout << "----------------------------------------\n";
    printTree(root);
    cout << "========================================\n\n";
}

int main(int argc, char *argv[])
{
    string path;
    if (argc > 1)
        path = argv[1];
    else
        path = askLine("Topic file name (e.g. chess.txt): ");

    string topic;
    Node *root = nullptr;

    ifstream probe(path);
    bool needsNew = !probe.good() || probe.peek() == EOF; // missing or empty
    probe.close();

    if (needsNew)
    {
        cout << "'" << path << "' is new or empty. Let's create a topic.\n";
        root = createNewTopic(topic);
        saveFile(path, topic, root);
    }
    else
    {
        root = loadFile(path, topic);
        if (!root)
        {
            cout << "'" << path << "' is not a valid topic file.\n"
                 << "I won't overwrite it. Fix it, or choose another file name.\n";
            return 1;
        }
    }

    if (askYesNo("Do you want to view the tree hierarchy?"))
    {
        viewTreeStats(topic, path, root);
    }

    cout << "\nTopic: " << topic << ". Think of one and I'll try to guess!\n\n";
    do
    {
        playRound(root);
        saveFile(path, topic, root);
    } while (askYesNo("Play again?"));

    delete root;
    return 0;
}
