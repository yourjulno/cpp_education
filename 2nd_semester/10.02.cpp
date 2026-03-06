#include <cassert>
#include <cstddef>
#include <deque>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

namespace deque_growth::exp
{
    struct AddressEvent
    {
        std::size_t index = 0;
        const int* address = nullptr;
        std::ptrdiff_t diff_from_prev = 0;
        bool new_block = false;
    };

    struct BlockInfo
    {
        std::size_t block_no = 0;
        std::size_t begin_index = 0;
        std::size_t end_index = 0;
        std::size_t element_count = 0;
        std::size_t bytes = 0;
    };

    static std::vector<AddressEvent> track_addresses(std::size_t n)
    {
        std::deque<int> d;
        std::vector<AddressEvent> events;
        events.reserve(n);

        const int* prev = nullptr;

        for (std::size_t i = 0; i < n; ++i)
        {
            d.push_back(static_cast<int>(i));
            const int* cur = &d.back();

            AddressEvent ev;
            ev.index = i;
            ev.address = cur;

            if (prev == nullptr)
            {
                ev.diff_from_prev = 0;
                ev.new_block = true;
            }
            else
            {
                ev.diff_from_prev = cur - prev;
                ev.new_block = (ev.diff_from_prev != 1);
            }

            events.push_back(ev);
            prev = cur;
        }

        return events;
    }

    std::vector<BlockInfo> detect_blocks(const std::vector<AddressEvent>& events)
    {
        std::vector<BlockInfo> blocks;
        if (events.empty()) return blocks;

        std::size_t block_start = 0;
        std::size_t block_no = 0;

        for (std::size_t i = 1; i < events.size(); ++i)
        {
            if (events[i].new_block)
            {
                BlockInfo b;
                b.block_no = block_no++;
                b.begin_index = events[block_start].index;
                b.end_index = events[i - 1].index;
                b.element_count = b.end_index - b.begin_index + 1;
                b.bytes = b.element_count * sizeof(int);
                blocks.push_back(b);

                block_start = i;
            }
        }

        BlockInfo last;
        last.block_no = block_no;
        last.begin_index = events[block_start].index;
        last.end_index = events.back().index;
        last.element_count = last.end_index - last.begin_index + 1;
        last.bytes = last.element_count * sizeof(int);
        blocks.push_back(last);

        return blocks;
    }

    void print_address_events(const std::vector<AddressEvent>& events, std::size_t max_rows = 64)
    {
        std::cout << "Address trace\n";
        std::cout << std::left
                  << std::setw(10) << "index"
                  << std::setw(20) << "address"
                  << std::setw(14) << "diff"
                  << std::setw(12) << "new_block"
                  << "\n";

        std::cout << std::string(56, '-') << "\n";

        const std::size_t rows = std::min(max_rows, events.size());
        for (std::size_t i = 0; i < rows; ++i)
        {
            const auto& e = events[i];
            std::cout << std::left
                      << std::setw(10) << e.index
                      << std::setw(20) << static_cast<const void*>(e.address)
                      << std::setw(14) << e.diff_from_prev
                      << std::setw(12) << (e.new_block ? "yes" : "no")
                      << "\n";
        }

        if (rows < events.size())
        {
            std::cout << "... (" << (events.size() - rows) << " more rows)\n";
        }

        std::cout << "\n";
    }

    static void print_blocks(const std::vector<BlockInfo>& blocks)
    {
        std::cout << "Detected contiguous blocks\n";
        std::cout << std::left
                  << std::setw(10) << "block"
                  << std::setw(14) << "begin_idx"
                  << std::setw(14) << "end_idx"
                  << std::setw(14) << "elements"
                  << std::setw(14) << "bytes"
                  << "\n";

        std::cout << std::string(66, '-') << "\n";

        for (const auto& b : blocks)
        {
            std::cout << std::left
                      << std::setw(10) << b.block_no
                      << std::setw(14) << b.begin_index
                      << std::setw(14) << b.end_index
                      << std::setw(14) << b.element_count
                      << std::setw(14) << b.bytes
                      << "\n";
        }

        std::cout << "\n";
    }

    void run_stability_test(std::size_t initial_size, std::size_t extra_pushes)
    {
        std::deque<int> d;
        for (std::size_t i = 0; i < initial_size; ++i)
        {
            d.push_back(static_cast<int>(i));
        }

        std::vector<const int*> saved;
        saved.reserve(d.size());
        for (std::size_t i = 0; i < d.size(); ++i)
        {
            saved.push_back(&d[i]);
        }

        for (std::size_t i = 0; i < extra_pushes; ++i)
        {
            d.push_back(static_cast<int>(initial_size + i));
        }

        std::size_t moved = 0;
        for (std::size_t i = 0; i < initial_size; ++i)
        {
            if (saved[i] != &d[i])
            {
                ++moved;
            }
        }

        std::cout << "Stability test\n";
        std::cout << "initial_size = " << initial_size << "\n";
        std::cout << "extra_pushes = " << extra_pushes << "\n";
        std::cout << "moved existing element addresses = " << moved << "\n\n";
    }

    void run_basic_tests(const std::vector<BlockInfo>& blocks)
    {
        assert(!blocks.empty());

        for (const auto& b : blocks)
        {
            assert(b.element_count > 0);
            assert(b.bytes == b.element_count * sizeof(int));
        }

        if (blocks.size() >= 2)
        {
            const std::size_t ref = blocks[1].element_count;
            for (std::size_t i = 1; i + 1 < blocks.size(); ++i)
            {
                assert(blocks[i].element_count == ref);
            }
        }
    }
}

int main(int argc, char** argv)
{
    std::size_t n = 300;
    if (argc >= 2)
    {
        try
        {
            n = static_cast<std::size_t>(std::stoull(argv[1]));
        }
        catch (...)
        {
            std::cerr << "Usage: " << argv[0] << " [N]\n";
            return 2;
        }
    }

    const auto events = deque_growth::exp::track_addresses(n);
    const auto blocks = deque_growth::exp::detect_blocks(events);

    deque_growth::exp::print_address_events(events);
    deque_growth::exp::print_blocks(blocks);
    deque_growth::exp::run_basic_tests(blocks);
    deque_growth::exp::run_stability_test(128, 5000);

    if (blocks.size() >= 2)
    {
        std::cout << "Estimated payload page size for int:\n";
        std::cout << "  elements per full block ~= " << blocks[1].element_count << "\n";
        std::cout << "  bytes per full block    ~= " << blocks[1].bytes << "\n";
    }
    else if (!blocks.empty())
    {
        std::cout << "Only one block observed.\n";
        std::cout << "Increase N to see page transitions.\n";
    }

    return 0;
}