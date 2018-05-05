#include <wav2midi/audio_stream.hpp>
#include <wav2midi/fft.hpp>
#include <wav2midi/scale.hpp>
#include <iostream>
#include <fstream>

int main() {
    std::ofstream ofs("data.dat");
    wav2midi::audio_stream as("../test/fixture/sound/c4_d4.wav");
    wav2midi::scale scale;

    as.read(std::pow(2, 12), [&scale, &ofs](auto f_s) {
        auto F_s = wav2midi::fft(f_s).execute();
        const auto N = F_s.size();
        auto max_amp = 0;
        auto max_freq = 0.0;

        for (auto k = 0u; k < N; ++k) {
            auto freq = 44.1 * 1000 * k / N;
            if (freq < 160) continue;
            if (freq > 5000) continue;

            auto amp = std::abs(F_s[k]);
            ofs << freq << " " << amp << std::endl;
            if (max_amp < amp) {
                max_amp = amp;
                max_freq = freq;
            }
            ofs << freq << " " << amp << std::endl;
        }
        std::cout << "========================" << std::endl;

        auto item = scale.match(max_freq);

        std::cout << item.name << ": " << max_freq << "Hz => "<< max_amp << std::endl;

        return false;
    });

    return 0;
}
