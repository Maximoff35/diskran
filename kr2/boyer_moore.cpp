#include <algorithm>
#include <array>
#include <string>
#include <vector>

std::vector<int> good_suffix_shifts(const std::string& p) {
    int m = static_cast<int>(p.size());
    std::vector<int> good(m + 1, 0);
    std::vector<int> border(m + 1);

    int i = m, j = m + 1;
    border[m] = m + 1;

    // Ищем другое вхождение совпавшего хвоста,
    // перед которым стоит другая буква.
    while (i > 0) {
        while (j <= m && p[i - 1] != p[j - 1]) {
            if (good[j] == 0) {
                good[j] = j - i;
            }
            j = border[j];
        }
        --i;
        --j;
        border[i] = j;
    }

    // Если подходящего вхождения нет, совмещаем
    // конец совпавшего хвоста с началом паттерна.
    j = border[0];
    for (i = 0; i <= m; ++i) {
        if (good[i] == 0) {
            good[i] = j;
        }
        if (i == j) {
            j = border[j];
        }
    }

    return good;
}

std::vector<int> boyer_moore(const std::string& text,
                             const std::string& pattern) {
    int n = static_cast<int>(text.size());
    int m = static_cast<int>(pattern.size());
    std::vector<int> positions;
    if (m == 0 || m > n) {
        return positions;
    }

    std::array<int, 256> last;
    last.fill(-1);
    for (int j = 0; j < m; ++j) {
        last[static_cast<unsigned char>(pattern[j])] = j;
    }

    std::vector<int> good = good_suffix_shifts(pattern);

    for (int start = 0; start <= n - m; ) {
        int j = m - 1;

        while (j >= 0 && pattern[j] == text[start + j]) {
            --j;
        }

        if (j < 0) {
            positions.push_back(start);
            start += good[0];
        } else {
            auto c = static_cast<unsigned char>(text[start + j]);
            int bad = std::max(1, j - last[c]);
            start += std::max(bad, good[j + 1]);
        }
    }

    return positions;
}