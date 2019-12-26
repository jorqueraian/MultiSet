#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <algorithm>

using namespace std;

struct word
{
	std::string name{ " " };
	int count{ 1 };
	word(std::string w, int c)
	{
		name = w;
		count = c;
	}
	word() = default;
};

class word_handler
{
private:
	//array vars
	int size_ = 100;
	int size_to_ignore_ = 50;
	int num_words_ = 0;
	int num_words_to_ignore_ = 0;
	int total_words_ = 0;
	int num_doubled_ = 0;

	//arrays
	word *words_;
	std::string *words_to_ignore_array_;

	//arrays sorting
	int top_words_to_output_ = 0;
	int lowest_in_top_ = 1;

	//streams
	std::ifstream hunger_games_;
	std::ifstream words_to_ignore_;

	//clocks
	std::chrono::steady_clock::time_point start;
	std::chrono::steady_clock::time_point finish;

	void add_to_ig_array(const std::string line)
	{
		bool new_word = true;
		for (int i = 0; i < num_words_to_ignore_; i++)
		{
			if (line == words_to_ignore_array_[i])
			{
				new_word = false;
				break;
			}
		}

		if (new_word)
		{
			if (num_words_to_ignore_ == size_to_ignore_)
			{
				size_to_ignore_ *= 2;
				const auto a = new std::string[size_to_ignore_];
				for (int i = 0; i < num_words_to_ignore_; i++)
				{
					a[i] = words_to_ignore_array_[i];
				}
				delete[]words_to_ignore_array_;
				words_to_ignore_array_ = a;
			}
			words_to_ignore_array_[num_words_to_ignore_] = line;
			num_words_to_ignore_++;
		}

	}

	void add_to_array(std::string line)
	{
		if (is_stop_word(line))
			return;

		total_words_++;

		for (int i = 0; i < num_words_; i++)//searches through existing array of words
		{
			if (line == words_[i].name)
			{
				words_[i].count++;

				if (words_[i].count > lowest_in_top_)
				{
					sort_top(i);
				}
				return;
			}
		}

		if (num_words_ == size_)
		{
			size_ = size_ * 2;
			auto *a2 = new word[size_];
			for (int i = 0; i < num_words_; i++)//memcpy
			{
				a2[i] = words_[i];
			}
			delete[] words_;
			words_ = a2;
			num_doubled_++;
		}

		words_[num_words_] = word(line, 1);
		num_words_++;
	}

	bool is_stop_word(const std::string line)
	{
		for (int i = 0; i < num_words_to_ignore_; i++)
		{
			if (line == words_to_ignore_array_[i])
			{
				return true;
			}
		}
		return false;
	}

	void sort_top(const int j)
	{
		if (num_words_ > top_words_to_output_ && j != 0)
		{
			if (j <= top_words_to_output_)
			{
				if (words_[j].count > words_[j - 1].count)
				{
					const auto temp = words_[j];
					words_[j] = words_[j + -1];
					words_[j - 1] = temp;
				}
			}
			else
			{
				std::sort(&words_[0], &words_[top_words_to_output_ - 1], [](const word& a, const word& b)
				{
					return (a.count > b.count);
				});

				if (words_[j].count > words_[top_words_to_output_ - 1].count)
				{
					const auto temp = words_[j];
					words_[j] = words_[top_words_to_output_ - 1];
					words_[top_words_to_output_ - 1] = temp;

				}
			}
			lowest_in_top_ = words_[top_words_to_output_ - 1].count;
		}
	}

	static void output_word(const std::string line, word words[], int num_words)
	{
		int count = 0;
		for (int i = 0; i < num_words; i++)
		{
			if (words[i].name.find(line) != std::string::npos)
			{
				count += words[i].count;
				//std::cout << words[i].name << ": " << words[i].count << std::endl;
			}
		}
		std::cout << line << ": " << count << std::endl;
	}

public:
	word_handler(std::string book, std::string words_to_ignore_file, int top_words_to_output)
	{
		words_ = new word[size_];
		words_to_ignore_array_ = new std::string[size_to_ignore_];

		hunger_games_.open(book);
		words_to_ignore_.open(words_to_ignore_file);
		top_words_to_output_ = top_words_to_output;
	}

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

	void load_words_to_ignore()
	{
		std::string line;

		while (std::getline(words_to_ignore_, line, '\n'))
		{
			add_to_ig_array(line);
		}
	}

	void read_book()
	{
		std::string line;
		while (hunger_games_ >> line)//std::getline(hunger_games, line, ' ')
		{
			add_to_array(line);
		}
	}

	void output_results(std::string str)
	{
		std::stringstream ss(str);
		std::string line;
		while (std::getline(ss, line, ','))
		{
			output_word(line, words_, num_words_);
		}

		std::cout << "Total words: " << total_words_ << std::endl;
		std::cout << "Total unique words: " << num_words_ << std::endl;
		std::cout << "Time Doubled: " << num_doubled_ << std::endl;
		std::cout << "Most common words:" << std::endl;
		for (int i = 0; i < top_words_to_output_; i++)
		{
			std::cout << words_[i].name << " - " << words_[i].count << std::endl;
		}

	}
};