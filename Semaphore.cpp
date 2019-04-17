#include <iostream>      
#include <thread>
#include <vector>
#include <mutex>         
#include <condition_variable>

using namespace std;

//--------------------------------
std::mutex mtx;             // mutex for critical section
std::condition_variable cv; // condition variable for critical section  
bool ready = false;         // Tell threads to run
int current = 0;            // current count
//Some variable needed by the semaphore
mutex semaphoreMutex;
condition_variable semaphoreFull;
condition_variable semaphoreEmpty;

//--------------------------------

int numberOfProducts = 10000;
int sharedMemorySize = 32; // Must be N -OK
vector<int> sharedMemory(sharedMemorySize,0);  //Initializes the Vector with 0 - OK
vector<int> consumerLocalMemory(numberOfProducts);

int numberOfProductsProduced = 0;
int numberOfProductsConsumed = 0;

int isPrime(int number)
{   
  for(int i = 2; i <= number/2; ++i)
  {
    if(number % i == 0)
        {
          return 0;
        }
  }
  return 1;
}

int generateRandomNumber(int minRandom, int maxRandom)
{   
    if ((minRandom <=0) or (maxRandom <=0))
    {
      perror("ERROR Sending Message to Socket file Descriptor");
      return EXIT_FAILURE;
    }
    srand(time(NULL));
    int outPut = rand()%((maxRandom - minRandom) + 1) + minRandom; 
    return outPut;
};

int getFirstMemoryFreePosition(vector<int> bufferMemory) 
{
  for (auto iterator = bufferMemory.begin(); iterator != bufferMemory.end(); ++iterator)
  {
    if (*iterator == 0)
    {
      return (iterator - bufferMemory.begin());
    }
  }
  return -1;      
}

int getFirstMemoryBusyPosition(vector<int> bufferMemory) 
{
  for (auto iterator = bufferMemory.begin(); iterator != bufferMemory.end(); ++iterator)
  {
    if (*iterator != 0)
    {
      return (iterator - bufferMemory.begin());
    }
  }
  return -1;    
}

bool isMemoryFull(vector<int> bufferMemory) 
{
  if(getFirstMemoryFreePosition(bufferMemory) == -1 )
  {
    return true;
  }
  else
    return false;  
}

bool isMemoryEmpty(vector<int> bufferMemory) 
{
  if(getFirstMemoryBusyPosition(bufferMemory) == -1 )
  {
    return true;
  }
  else
    return false;  
}


void print_num(int num, int max) {

  std::unique_lock<std::mutex> lck(mtx);
  while(num != current || !ready){ cv.wait(lck); }
  current++;
  std::cout << "Thread: ";
  std::cout << num + 1 << " / " << max;
  std::cout << " current count is: ";
  std::cout << current << std::endl;
  
  /* Notify next threads to check if it is their turn */
  cv.notify_all(); 
}

/* Changes ready to true, and begins the threads printing */
void run(){
  std::unique_lock<std::mutex> lck(mtx);
  ready = true;
  cv.notify_all();
}
 
int main (int argc, char *argv[])
{
  int nP = 0 , nC = 0;
  if (argc < 3) 
  {
    fprintf(stderr,"Usage %s : NumberOfProducer NumberOfConsumer NumberOfProductions \n", argv[0]);
    exit(0);
  }
  nP = atoi(argv[1]);
  nC = atoi(argv[2]);
}



  int threadnum = 15;
  std::thread threads[15];

  /* spawn threadnum threads */
  for (int id = 0; id < threadnum; id++)
    threads[id] = std::thread(print_num, id, threadnum);

  std::cout << "\nRunning " << threadnum;
  std::cout << " in parallel: \n" << std::endl;

  run(); // Allows threads to run

  /* Merge all threads to the main thread */
  for(int id = 0; id < threadnum; id++)
    threads[id].join();

  std::cout << "\nCompleted semaphore example!\n";
  std::cout << std::endl;

  return 0;
