#include <iostream>
#include <string>
#include <list>
#include <vector>

using namespace std;

typedef unsigned short int small_int;

/// Mapping of characters to digital keys
string char_keys[26] = {"2", "2", "2",
                        "3", "3", "3",
                        "4", "4", "4",
                        "5", "5", "5",
                        "6", "6", "6",
                        "7", "7", "7", "7",
                        "8", "8", "8",
                        "9", "9", "9", "9"};

const string& get_key(const char& c)
{
    return char_keys[c - 'a'];
}

string get_full_key(const string& s)
{
    string res;
    
    for (string::const_iterator i = s.begin(); i != s.end(); i++)
        res += get_key(*i);

    return res;
}

/// Word with frequency
class Word
{
    string str;
    small_int frequency;
    
public:
    Word()
    {}
    
    Word(const string &s, const small_int &freq)
    {
        str = s;
        frequency = freq;
    }

    void send(ostream &out)
    {
        out << str;
    }    
};

ostream& operator <<(ostream &out, Word &w)
{
    w.send(out);
    return out;
}

class Trie
{
private:
    /// Words stored in root leaf of trie
    list<Word> words;

    /// Children tries
    vector<Trie*> children;
    
    /// Add word contents with given full key and frequency
    void add_word_proc(string &full_key, const string &contents, const small_int &freq)
    {
        vector<Trie*>::size_type key = (full_key[0] - '2');
        if (!full_key.length())
            words.push_back(Word(contents, freq));
        else
        {
            full_key.erase(0, 1);
            if (children[key] == NULL)
                children[key] = new Trie();
            children[key]->add_word_proc(full_key, contents, freq);
        }
    }
public:
    Trie(void)
    {
        /// Preallocate vector for 8 (from 2 to 9) children which may
        /// be added later
        children.resize(8, NULL);
    }

    ~Trie(void)
    {
        for (vector<Trie*>::iterator i = children.begin(); i != children.end(); i++)
            if (*i != NULL)
                delete *i;
    }

    /// @internal Public wrapper for add_word_proc
    void add_word(const string &contents, const small_int &freq)
    {
        string fk = get_full_key(contents);
        add_word_proc(fk, contents, freq);
    }

    /// Get list of words stored in trie under given full key. We
    /// assume that all used words are present in the trie, so this
    /// always succeeds.
    const list<Word>& query(string &full_key)
    {
        vector<Trie*>::size_type key = (full_key[0] - '2');
        if (!full_key.length())
            return words;
        else
        {
            full_key.erase(0, 1);
            return children[key]->query(full_key);
        }
    }
};

int main(int argc, char* argv[])
{
    int dict_size;
    small_int freq;
    Trie tr;
    string word, full_key;
    Word w;

    cin >> dict_size;

    /// Populate trie
    for (int i = 0; i < dict_size; i++)
    {
        cin >> word >> freq;
        tr.add_word(word, freq);
    }

    cin >> full_key;
    w = tr.query(full_key).front();
    cout << w;

    return 0;
}
