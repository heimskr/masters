let i = 0;
while (i < 100) {
	if (++i % 2 === 0)
		continue;
	print(i);
	if (i === 91)
		break;
}
