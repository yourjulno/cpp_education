#include <algorithm>
#include <cassert>
#include <cstddef>
#include <vector>

auto find(std::vector<int> const& vector_1, std::vector<int> const& vector_2)
{
	const std::size_t n = vector_1.size();
	const std::size_t m = vector_2.size();

	std::vector<std::vector<std::size_t>> dp(
		n + 1, std::vector<std::size_t>(m + 1, 0)
	);

	std::size_t best_len = 0;
	std::size_t best_end_i = 0;

	for (std::size_t i = 1; i <= n; ++i)
	{
		for (std::size_t j = 1; j <= m; ++j)
		{
			if (vector_1[i - 1] == vector_2[j - 1])
			{
				dp[i][j] = dp[i - 1][j - 1] + 1;

				if (dp[i][j] > best_len)
				{
					best_len = dp[i][j];
					best_end_i = i;
				}
			}
			else
			{
				dp[i][j] = 0;
			}
		}
	}

	std::vector<int> result;
	result.reserve(best_len);

	const std::size_t start = best_end_i - best_len;
	for (std::size_t k = start; k < best_end_i; ++k)
	{
		result.push_back(vector_1[k]);
	}

	return result;
}

int main()
{

	assert(find({1, 2, 3, 4, 5}, {1, 3, 5}) == std::vector<int>({1}));

	assert(find({1, 2, 3, 4, 5}, {9, 2, 3, 4, 8}) == std::vector<int>({2, 3, 4}));

	assert(find({1, 2}, {3, 4}).empty());
}