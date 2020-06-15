import sys
import random
import argparse

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('file', nargs='?', type=argparse.FileType('r'))
    parser.add_argument('--head-count', '-n', type=int, help='outputs a certain head-count of lines')
    parser.add_argument('--input-range', '-i', type=str, help='outputs numbers in input-range')
    parser.add_argument('--repeat', '-r', action='store_true', help='allows repetition')
    args = parser.parse_args()

if args.input_range is not None and args.file is not None:
    print("Cannot provide both file and input-range")
    sys.exit(1)

#read the file's lines into a list
if (args.file):
    count = 0
    line_list = []
    line = args.file.readline().rstrip('\n')

    while line:
        line_list.append(line)
        count += 1
        line = args.file.readline().rstrip('\n')

    if len(line_list) == 0:
        sys.exit(1)
    
#create list based on user's input_range
elif args.input_range is not None:
    range_list = args.input_range.split('-')

    if int(range_list[0]) < 0 or int(range_list[1]) < 0 or int(range_list[1]) < int(range_list[0]):
        print("Invalid input range")
        sys.exit(1)
    if len(range_list) > 2:
        print("Invalid input range")
        sys.exit(1)

    count = int(range_list[1]) - int(range_list[0]) + 1
    line_list = list(range(int(range_list[0]), int(range_list[1]) + 1))

#read from standard input
else:
    line_list = []
    count = 0
    for line in sys.stdin:
        line_list.append(line.rstrip('\n'));
        count += 1

    if len(line_list) == 0:
        sys.exit(1)

#create list with random sampling and repetition
if args.repeat:
    shuf_list = random.choices(line_list, k=count)

    #implement infinite repeating loop
    if args.head_count is None:
        infinite = 0
        while infinite < 1:
            shuf_list = random.choices(line_list, k=count)
            print(*shuf_list, sep = '\n')

#create list with random sampling
else:
    shuf_list = random.sample(line_list, k=count)

#only print head_count lines
if args.head_count is not None:
    output_num = 0
    while output_num < args.head_count:
        print(shuf_list[output_num])
        output_num += 1
else:
    print(*shuf_list, sep = '\n')
