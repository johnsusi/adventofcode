runtime = osx-x64
cc = clang++ -Wall

years = 2017 2018 2019 2020 2021
days = 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25

targets = $(foreach year,$(years), $(addprefix $(year)-day, $(days)))

.PHONY = $(targets) $(addsuffix -debug,$(targets)) $(addsuffix -rs,$(targets)) clean

$(addsuffix -debug, $(targets)): %:
	@pushd $(subst -,/,$(@:%-debug=%)) \
	&& $(cc) -std=c++20 -I../../ -g -omain main.cpp \
	&& lldb ./main

$(addsuffix -cpp, $(targets)): %:
	mkdir -p build/cpp/release \
	&& $(cc) -std=c++20 -I. -O2 -obuild/cpp/release/$(@:%-cpp=%) $(subst -,/,$(@:%-cpp=%))/main.cpp \
	&& time ./build/cpp/release/$(@:%-cpp=%)

$(addsuffix -rs, $(targets)): %:
	cargo build --bin $(@:%-rs=%) --target-dir build/rust --release \
	&& time ./build/rust/release/$(@:%-rs=%)

2021 : $(addprefix 2021-day, $(days))

clean:
	@find . -name *.dSYM -type d | xargs rm -Rf
