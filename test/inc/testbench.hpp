template<class T>
class TestBench
{
    T* model;
public:
    void tick();
    TestBench(T* modelIn);
};

template<class T>
void TestBench<T>::tick()
{
    model->eval();
}

template<class T>
TestBench<T>::TestBench(T* modelIn): model(modelIn)
{

}
