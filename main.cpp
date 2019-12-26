#include <iostream>
#include <string>
#include "multi_set.hpp"
#include "word_array.hpp"
#include <functional>
#include "trie_tree.hpp"
#include "treap.hpp"

using namespace std;

#pragma region Clocks and Timing
//clocks
std::chrono::steady_clock::time_point start;
std::chrono::steady_clock::time_point finish;

void start_timer()
{
	start = std::chrono::high_resolution_clock::now();
}
void stop_timer()
{
	finish = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = finish - start;
	std::cout << "Elapsed time: " << elapsed.count() << " s\n";
}

#pragma endregion 

//HungerGames_edit.txt ignoreWords.txt Hamlet_edited.txt

int main(int argc, char *argv[])
{
	#pragma region Array

	auto wh = word_handler(argv[3], argv[2], 10);// creates word handler class with hamlet and words to ignore text files

	wh.start_timer();
	wh.load_words_to_ignore();
	wh.read_book();

	wh.output_results("hamlet");// outputs top 10 words and how common hamlet is, keep in mind that this result will be higher than the amount of "hamlet" as it will include "hamlet's" etc

	wh.stop_timer();

	system("pause");
	cout << endl;
	#pragma endregion

	#pragma region Set

	start_timer();
	std::string line;

	ifstream hunger_games_ifs(argv[1]);
	ifstream words_to_ignore(argv[2]);
	ifstream hamlet_in(argv[3]);

	//Creates multiset for both books and words to ignore
	multi_set<string> hunger_games(6277);
	multi_set<string> hamlet(3793);
	multi_set<string> ignore(37, true);

	//read in each book adding every word to multi_sets
	while (words_to_ignore >> line)
	{
		ignore.add(line);
	}
	while (hunger_games_ifs >> line)//this was commented out for hamlet test
	{
		hunger_games.add(line);
	}
	while (hamlet_in >> line)
	{
		hamlet.add(line);
	}

	cout << "Unique Words for hunger games: " << hunger_games.get_size() << endl;
	cout << "Unique Words for Hamlet: " << hamlet.get_size() << endl;

	auto subtract_set_hg = hunger_games.set_subtraction(ignore);//performs subtraction, removing the words to ignore 
	auto subtract_set_h = hamlet.set_subtraction(ignore);

	cout << "Unique non-top 50 Words for hunger games: " << subtract_set_hg->get_size() << endl;
	cout << "Unique non-top 50 Words for hamlet: " << subtract_set_h->get_size() << endl;

	auto intersection_set = subtract_set_hg->set_intersection(*subtract_set_h);//finds what words are in both hamlet and hunger games

	cout << "words in both hamlet and hunger games, not including top 50: " << intersection_set->get_size() << endl;

	auto union_set = subtract_set_hg->set_union(*subtract_set_h);//finds total words, in either hunger games or hamlet or both

	cout << "all unique words in hamlet and hunger games, not including top 50: " << union_set->get_size() << endl;

	stop_timer();
	cout << endl;
	system("pause");
	#pragma endregion

	#pragma region Treap

	hunger_games_ifs.close();//reload file streams
	words_to_ignore.close();
	hamlet_in.close();

	hunger_games_ifs.open(argv[1]);
	words_to_ignore.open(argv[2]);
	hamlet_in.open(argv[3]);

	start_timer();

	treap<string> string_treap;
	while (hamlet_in >> line)//addes every words
	{
		string_treap.insert_node(line);
	}

	//outputs the number of hamlet in the book
	cout << "there are " << string_treap.search_i("hamlet")->count
		<< " instances of the word hamlet!" << endl;

	//not in the presentation,added 12/14/18, but to sort this data you can pop each item into a vector and then use stl sort
	//auto hamlet_treap_sorted = string_treap.sort_data();
	//cout << hamlet_treap_sorted[8].first << " " << hamlet_treap_sorted[8].second << endl; //for some reason this doesnt work when i try to output but does when i dont

	stop_timer();
	cout << endl;
	system("pause");

	#pragma endregion 

	#pragma region TrieTree

	hunger_games_ifs.close();//reload file streams
	words_to_ignore.close();
	hamlet_in.close();

	hunger_games_ifs.open(argv[1]);
	words_to_ignore.open(argv[2]);
	hamlet_in.open(argv[3]);

 	start_timer();

	trie trie_tree;
	while (hamlet_in >> line)
	{
		trie_tree.add_word(line);
	}


	cout << "there are " << trie_tree.get_size() << " total words\nAnd " << trie_tree.has_word("hamlet")
		<< " instances of the word hamlet!" << endl;
	stop_timer();
	cout << endl;

	#pragma endregion

	system("pause");
	return 0;
}



//The Code used to clean Hamlet:
    /*
     *ifstream hamlet_in_unedited(argv[3]);
	ofstream hamlet_out;
	hamlet_out.open("Hamlet_edited.txt");
	if (hamlet_out.is_open())
	{

		while (hamlet_in >> line)
		{

			for (int i = 0, len = line.size(); i < len; i++)
			{
				line.at(i) = tolower(line.at(i));

				if (ispunct(line[i]))
				{
					line.erase(i--, 1);
					len = line.size();
				}

			}

			hamlet_out << line << " ";
		}
	}

     */