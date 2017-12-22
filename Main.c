code Main

  -- OS Class: Project 3
  --
  -- <Mrunal Hirve>
  --

-----------------------------  Main  --------------------------------- 

 function main ()
      print ("Example Thread-based Programs...\n")

      InitializeScheduler ()
	--SleepingBarber() 
	GamingParlors ()
      ThreadFinish ()

    endFunction
--------------------------------Sleeping barber Implementation-----------------------------------------
const
	CHAIRS= 5										--Declaring the number of chairs in barber shop

enum E,S,B,F,L,start,end,X,n									--Enum maintaining status
var 
	customersThreads: array [12] of Thread = new array of Thread { 12 of new Thread }	--creating array of customer threads
	waiting:int=0 										--global variable maintaining the customer waiting for haircut
	customerSemaphore: Semaphore=new Semaphore						--creating binary semaphore for customer
	barberSemaphore: Semaphore=new Semaphore						--creating binary semaphore for barber
	mutexSemaphore: Semaphore=new Semaphore  						--creating binary semaphore for locking
	barberThread: Thread									--creating barber threads
	customers: int=0									
	barber: int=0
	mutex: int=1
	status: array[12] of int								--maintains status of each thread				
	numberOfChairs:array[5] of int								--maintains status of each chair
	index: int=0										--index for chair array

  function SleepingBarber()

	print("       Barber     1  2  3  4  5  6  7  8  9  10  11  \n")

	status= new array of int {12 of '?'}							--initializes status of thread					
	numberOfChairs=new array of int {5 of n }   						--initializes numberOfChairs with n
	customerSemaphore.Init(customers)							--initializing customer semaphore 
	barberSemaphore.Init(barber)								--initializing barber semaphore
	mutexSemaphore.Init(mutex)								--initializing mutex semaphore
	barberThread=new Thread									


	barberThread.Init("Barber")								--initializing barber thread
        barberThread.Fork(Barber,0)	

	customersThreads[0].Init ("C1")								--initializes all customer threads								
        customersThreads[0].Fork (Customer, 1)

	customersThreads[1].Init ("C2")
        customersThreads[1].Fork (Customer, 2)

	customersThreads[2].Init ("C3")
        customersThreads[2].Fork (Customer, 3)

        customersThreads[3].Init ("C4")
        customersThreads[3].Fork (Customer, 4)

	customersThreads[4].Init ("C5")
        customersThreads[4].Fork (Customer, 5)

        customersThreads[5].Init ("C6")
        customersThreads[5].Fork (Customer, 6)

											--adds delay before customer 7 arrives 			

        customersThreads[6].Init ("C7")
        customersThreads[6].Fork (Customer, 7)

        loopWait(1000)										--adds delay before customer 7 arrives 			

        customersThreads[7].Init ("C8")	
        customersThreads[7].Fork (Customer, 8)


      --loopWait(100)
	

        customersThreads[8].Init ("C9")
        customersThreads[8].Fork (Customer, 9)

        customersThreads[9].Init ("C10")
        customersThreads[9].Fork (Customer, 10)
	
      --loopWait(100)

        customersThreads[10].Init ("C11")
        customersThreads[10].Fork (Customer, 11)
	
	ThreadFinish()

  endFunction

 function Barber(p:int)										--barber method 
	
	while true
		customerSemaphore.Down()							--sleeps the barber only if there are no customer 
		mutexSemaphore.Down()								--acquiring lock for global variable waiting
		numberOfChairs[index]=n								--makes a customer chair empty when barber wakes up
		index=(index-1) % CHAIRS							--calculates the index for chair array
		waiting=waiting-1								--decrement count of waiting customers 
		barberSemaphore.Up()								--one barber is now ready to cut hair 
		mutexSemaphore.Up()								--releases the lock
		CutHair(p)									--barber starts cutting hair
	endWhile

 endFunction

 function Customer(p:int)									--customer method
		mutexSemaphore.Down()								--enter critical section
		status[p]=E									--when a customer enters make his status as Enter- E
		PrintAllStatus(p)								--print the status of the customer
		if( waiting < CHAIRS)								--checks if waiting customer is less than chairs available
			numberOfChairs[index]=X							--makes the chair occupied-X					
			index=(index+1) % CHAIRS						--adds to the index caluclation for occupied chairs 
			waiting=waiting+1							--increments the numberof customers waiting to cut hair
			status[p]=S								--makes the status of the customer as sitting - S
                        PrintAllStatus(p)							--prints the status of the customer as sitting - S				
			customerSemaphore.Up()							--wakes up barber if necessary
			mutexSemaphore.Up()							--releases the critical section 
			barberSemaphore.Down()							--go to sleep when number of customer is 0
			GetHairCut(p)								--if a customer is available start cutting hair
                        status[p]=L								--makes the status leave -L when cutting hair is done
                        mutexSemaphore.Down()							--acquire lock before printing the status of for the customer
        		PrintAllStatus(p)							--prints the status for the customer
        		mutexSemaphore.Up()							--releases lock
		else										--else part - leaves when no chairs available
			status[p]=L								--makes the status as leave 
			PrintAllStatus(p)							--prints the status 
			mutexSemaphore.Up()							--releases the lock
		endIf
endFunction 

 function CutHair(p:int)									--barber cut hair method
	var i:int
	
        status[p]=start										--make the customer status as start when barber starts cutting
	mutexSemaphore.Down()									--acquires lock before printing
	PrintAllStatus(p)									--prints the status	
	mutexSemaphore.Up()									--releases the lock
	for i=i to 100										
		currentThread.Yield() 								--adds time for cutting hair, after done the willingly hands control
	endFor
	status[p]=end										--makes the customer status as end 
	mutexSemaphore.Down()									--acquires lock before printing
	PrintAllStatus(p)									--prints the status
	mutexSemaphore.Up()									--releases the lock
	 
endFunction

 function GetHairCut(p:int)									--cutomer get haircut method
	var i:int
	status[p]=B										--makes the status of the customer as begin-B
	mutexSemaphore.Down()									--acquires lock before printing
	PrintAllStatus(p)									--prints the status											
	mutexSemaphore.Up()									--releases the lock
	status[p]=F										--makes the status of the customer as finish - F
	mutexSemaphore.Down()									--acquires lock before printing
	PrintAllStatus(p)									--prints the status
	mutexSemaphore.Up()									--releases the lock
	for i=i to 120										--adds time for haircut
              currentThread.Yield()								--customer gives its control willingly when hair cut done
        endFor
endFunction

function loopWait(time: int)									--for adding delay 
    var i: int
    for i =  1 to time
    endFor
endFunction

function PrintAllStatus(j: int)									--method for printing status of each cutomer
var
	  i:int
          p: int
	
	for i=0 to 4										--prints the status of chairs occupied or not occupied
	  switch numberOfChairs[i]
	    case X:
		print("X")
		break
	    case n:
		print("0")
		break
	 endSwitch
	endFor
	
        if j != 0
           print ("          ")
        endIf

	for p = 1 to j										--prints the status of the cutomer 
            print ("   ")
        endFor  
          switch status [j]
            case E:
              print ("E  ")
              break
            case S:
              print ("S  ")
              break
            case B:
              print ("B  ")
              break
	    case F:
	      print ("F  ")
	      break
            case L:
	      print ("L  ")
	      break

	    case start:
	      print ("  Start   ")
	      break
	    case end:
	     print ("  End     ")
	     break
	  default: print ("   ")
          endSwitch
        nl ()
      endFunction
----------------------------------------GameParlour-------------------------------------------------------------------------


var
    mon: Game											--declare a monitor object for game 
    gamegroup: array [8] of Thread = new array of Thread {8 of new Thread }			--creates array for game group
	
function GamingParlors ()									--method gaming parlour begins

      mon = new Game										--creates the monitor object
      mon.Init ()										--initializes the monitor

      gamegroup[0].Init ("A")									--initializes each gamegroup thread 
      gamegroup[0].Fork (PlayingGroup, 4)
      
      gamegroup[1].Init ("B")
      gamegroup[1].Fork (PlayingGroup, 4)

      gamegroup[2].Init ("C")
      gamegroup[2].Fork (PlayingGroup, 5)

      gamegroup[3].Init ("D")
      gamegroup[3].Fork (PlayingGroup, 5)

      gamegroup[4].Init ("E")
      gamegroup[4].Fork (PlayingGroup, 2)

      gamegroup[5].Init ("F")
      gamegroup[5].Fork (PlayingGroup, 2)

      gamegroup[6].Init ("G")
      gamegroup[6].Fork (PlayingGroup, 1)

      gamegroup[7].Init ("H")
      gamegroup[7].Fork (PlayingGroup, 1)
endFunction
	 
function PlayingGroup (numOfDice: int)								--function to access monitor methods
     var
        i: int
      for i = 1 to 5										--for each group plays the game 5 times

        -- Now he is asking
        mon. RequestDice (numOfDice)								--accessing request dice method
		currentThread.Yield()								--after getting dice play ??

        -- Now he is Playing
        mon. ReturnDice (numOfDice)								--accessing return dice method
		currentThread.Yield()								--after playing return ??

      endFor
endFunction

class Game											--creates a class game as monitor
    superclass Object
    fields											--declaring fields of the class game
		moniterLock:Mutex								--declaring monitor lock
		front: Condition								--declaring condition variable for font of group	
		restGroup : Condition								--declaring condition variable for rest of group
		numOfDiceAvail : int								--variable for number of dice available
		numOfWaitingGroups : int							--variable for number of group waiting

    methods											--methods for game class
      Init ()											--declare initializing method
      RequestDice (numOfDiceNeeded: int)							--declaring request dice method of the monitor
      ReturnDice (numOfDiceReturned: int)							--declaring return dice method of the monitor
      Print (str:String, count:int)								--method for printing
endClass

behavior Game

    method Init()										--initializes values for the monitor class variables

		front = new Condition
		restGroup = new Condition
		front.Init()
		restGroup.Init()
		numOfDiceAvail = 8
		numOfWaitingGroups = 0
		moniterLock = new Mutex
		moniterLock.Init()

	endMethod

    method RequestDice (numOfDiceNeeded: int)							--requesting dice method
         
		moniterLock.Lock()           			        			--acquires lock
		self.Print("requests", numOfDiceNeeded)						--prints as requested and the number of dice requested
		numOfWaitingGroups = numOfWaitingGroups + 1            				--increases the count for waiting group	
		if numOfWaitingGroups > 1 							--if count of waiting group is greater than one, makes the restof group to wait
			restGroup.Wait(&moniterLock)                   
		endIf
		while numOfDiceAvail < numOfDiceNeeded   					--checks availablity of dice with the dice needed              
       			front.Wait(&moniterLock)						--makes the monitor wait if requested dice not available
		endWhile
		numOfDiceAvail = numOfDiceAvail - numOfDiceNeeded				--if dice available makes the count less of available dice
		numOfWaitingGroups = numOfWaitingGroups - 1					--decreases the count of waiting group
		restGroup.Signal (& moniterLock)						--signals rest of group 
		self.Print ("proceed", numOfDiceNeeded)						--prints the proceed request and now available dice presently after processing request
		moniterLock.Unlock()     							--releases lock                                 

    endMethod

    method ReturnDice (numOfDiceReturned: int)							--return dice method
            
      	moniterLock.Lock()            								--acquires lock    
	numOfDiceAvail = numOfDiceAvail + numOfDiceReturned 					--adds the number of available dice
	self.Print("release and adds back", numOfDiceReturned)					--releases and adds back the number of dice returned
	front.Signal(&moniterLock)								--signals the front of group that dice are available
	moniterLock.Unlock()              							-- moniters lock released

    endMethod


    method Print(str: String, count: int)							--prints request and return of dice of each group
		print(currentThread.name)
		print("")
		print(str)
		print("")
		printInt(count)
		nl ()
		print ("-----------------------------------Number of dice now avail = ")
		printInt (numOfDiceAvail)
		nl()
    endMethod

  endBehavior 
endCode

