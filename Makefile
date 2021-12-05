runtime = osx-x64
cc = clang++

years = 2019 2020 2021
days = 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25

targets = $(foreach year,$(years), $(addprefix $(year)-day, $(days)))

.PHONY = $(targets) $(addsuffix -debug,$(targets)) clean

$(targets): %:
	@pushd $(subst -,/,$@) \
	&& $(cc) -std=c++20 -I../../ -O2 -omain main.cpp \
	&& ./main

$(addsuffix -debug, $(targets)): %:
	@pushd $(subst -,/,$(@:%-debug=%)) \
	&& $(cc) -std=c++20 -I../../ -g -omain main.cpp \
	&& lldb ./main

clean:
	@find . -name *.dSYM -type d | xargs rm -Rf
