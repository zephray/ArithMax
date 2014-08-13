
.PHONY: all

all: ../../font-12.c ../../font-8.c ../../image-font-8.c

../../font-12.c: ArithFont-SBMonoH-Regular-12.bdf ConvertFont-font.h-12.py
	python3 ./ConvertFont-font.h-12.py <ArithFont-SBMonoH-Regular-12.bdf >../../font-12.c

../../font-8.c: ArithFont-SBMonoM-Regular-8.bdf ConvertFont-font.h-8.py
	python3 ./ConvertFont-font.h-8.py <ArithFont-SBMonoM-Regular-8.bdf >../../font-8.c

../../image-font-8.c: ArithFont-SBMonoM-Regular-8.bdf ConvertFont-image.h-8.py
	python3 ./ConvertFont-image.h-8.py <ArithFont-SBMonoM-Regular-8.bdf >../../image-font-8.c
