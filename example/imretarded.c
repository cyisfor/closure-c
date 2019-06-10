int main(int argc, char *argv[])
{
    int m;
	int a = 9*6;
	int b = 42;
	for(m=7;m<1000;++m) {
		/*
		  b = ((a + n) / m) + n)/m
		  m * b = (a + n) / m + n
		  m * m * b = a + n + n * m
		  m * m * b - a =  (1 + m) * n
		  n =  (m * m * b - a) /(1+m)
		 */
		int n = (m * m * b - a) / (1 + m);
		if(((a + n) / m + n) / m == b) {
			printf("n = %d, m = %d\n", n,m);
			return 0;
		}
	}
	return 23;
}
