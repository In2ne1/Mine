int main()
{
	String s1("aaaaa");
	cout << s1 << endl;
	cout << s1.append(4, 'n') << endl;
	cout << s1.append("bbb") << endl;
	
	cout << s1.find('n') << endl;


	String s2("Thanks for your help!");
	cout << s2 << endl;

	s2.insert("useful ", 15);  //const char* str, pos

	s2.insert("useful      ", 7, 15);  //const char* str, 前n, pos 
	cout << s2 << endl;


	String s4("aaaabbbbccccdddd");
	cout << s4 << endl;
	cout << s4.erase(4, 4) << endl;  //len, pos
	cout << s4.erase(2) << endl;  //pos


	String s5("s'tahw pu ew 1en2");

	cout << s5.reverse(s5) << endl; 

	system("pause");
	return 0;
}
