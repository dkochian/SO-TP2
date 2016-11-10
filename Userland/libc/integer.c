int strint(const char* str) {
	int
		result = 0,
		index = 0,
		current,
		flag = 0;

	while(str[index] != '\0') {
		if(index == 0 && str[index] == '-') {
			flag = 1;
			continue;
		}

		current = str[index] - '0';

		if(current < 0 || current > 9)
			return -1;

		result = result*10 + current;
		index++;
	}

	return (flag ? (-1)*result : result);
}

int strintPos(const char* str) {
	int
		result = 0,
		index = 0,
		current;

	while(str[index] != '\0') {

		current = str[index] - '0';

		if(current < 0 || current > 9)
			return -1;

		result = result*10 + current;
		index++;
	}

	return result;
}