// /chapter_number_processing/random_numbers_weasel.cpp

///////////////////////////////////////////////////////////////////////////////////////////
//
// chapter : Number Processing
//
///////////////////////////////////////////////////////////////////////////////////////////
//
// section : Random Numbers
//
///////////////////////////////////////////////////////////////////////////////////////////
//
// content : Dawkins' Weasel (Blind Watchmaker, ch. 3)
//
///////////////////////////////////////////////////////////////////////////////////////////

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <random>
#include <string>
#include <vector>

///////////////////////////////////////////////////////////////////////////////////////////

namespace
{
	constexpr auto kTarget = "methinksitislikeaweasel";
	constexpr std::size_t kLen = 23;
	constexpr std::size_t kPopulation = 100;
	constexpr double kMutationProb = 0.05;

	static_assert(kLen == 23);
	static_assert(std::string_view{kTarget}.size() == kLen);

	[[nodiscard]] char random_letter(std::default_random_engine& engine,
	                                 std::uniform_real_distribution<double>& dist01)
	{
		const auto r = dist01(engine);                // [0,1)
		const auto idx = static_cast<int>(r * 26.0);  // 0..25
		return static_cast<char>('a' + (idx < 26 ? idx : 25));
	}

	[[nodiscard]] std::string random_string(std::default_random_engine& engine,
	                                        std::uniform_real_distribution<double>& dist01)
	{
		std::string s(kLen, 'a');
		for (std::size_t i = 0; i < kLen; ++i)
		{
			s[i] = random_letter(engine, dist01);
		}
		return s;
	}

	[[nodiscard]] std::size_t metric(const std::string& s)
	{
		assert(s.size() == kLen);
		std::size_t d = 0;
		for (std::size_t i = 0; i < kLen; ++i)
		{
			d += (s[i] != kTarget[i]) ? 1u : 0u;
		}
		return d;
	}

	[[nodiscard]] std::string mutate_copy(const std::string& parent,
	                                      std::default_random_engine& engine,
	                                      std::uniform_real_distribution<double>& dist01)
	{
		std::string child = parent;
		for (std::size_t i = 0; i < kLen; ++i)
		{
			if (dist01(engine) < kMutationProb)
			{
				char c = random_letter(engine, dist01);
				while (c == child[i]) // "replace with another letter"
				{
					c = random_letter(engine, dist01);
				}
				child[i] = c;
			}
		}
		return child;
	}
} // namespace

///////////////////////////////////////////////////////////////////////////////////////////

int main()
{
	std::random_device device;
	std::default_random_engine engine(device());
	std::uniform_real_distribution<double> dist01(0.0, 1.0);

	auto parent = random_string(engine, dist01);
	std::size_t parent_metric = metric(parent);

	std::cout << "target : " << kTarget << '\n';
	std::cout << "start  : " << parent << "  (metric=" << parent_metric << ")\n";

	for (std::size_t iteration = 1;; ++iteration)
	{
		std::vector<std::string> population;
		population.reserve(kPopulation);

		for (std::size_t i = 0; i < kPopulation; ++i)
		{
			population.push_back(mutate_copy(parent, engine, dist01));
		}

		std::size_t best_metric = kLen + 1;
		std::string best_string;

		for (const auto& s : population)
		{
			const auto d = metric(s);
			if (d == 0)
			{
				std::cout << "iter " << iteration << " : " << s << "  (metric=0)\n";
				std::cout << "done  : " << kTarget << '\n';
				return 0;
			}
			if (d < best_metric)
			{
				best_metric = d;
				best_string = s;
			}
		}

		parent = best_string;
		parent_metric = best_metric;

		std::cout << "iter " << iteration << " : " << parent
		          << "  (metric=" << parent_metric << ")\n";
	}
}

///////////////////////////////////////////////////////////////////////////////////////////
