int main(int argc, char *argv[])
{
    int m;
	int a = 9*6;
	int b = 42;
	for(m=2;m<1000;++m) {
		/*
		  m * b = (a + n) / m + n
		  m * m * b - m * n = a+n
		  n = m * m * b - m * n - a
		 */
		int n = m * m * b - m * n - a;
		if(((a + n) / m + n) / m == b) {
			printf("n = %d, m = %d\n", n,m);
			return 0;
		}
	}
	return 23;
}
