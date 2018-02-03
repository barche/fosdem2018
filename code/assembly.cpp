template<typename A, typename B>
auto add(A a, B b) -> decltype(a + b)
{
    return a + b;
}

long add2(long a, long b)
{
    return add(a,b);
}

// int main()
// {
//     add2(1,2);
//     return 0;
// }