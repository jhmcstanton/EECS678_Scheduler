import System.Environment (getArgs)
import System.Process (createProcess, proc, StdStream( CreatePipe ), CreateProcess( .. ) )
import GHC.IO.Handle (hGetContents)

data Result = Result {
  outLine    :: String,
  response   :: Double,
  turnaround :: Double, 
  wait       :: Double
  } deriving (Show, Eq)

allSchemes = ["fcfs", "sjf", "psjf", "pri", "ppri", "rr1", "rr2", "rr4"]

getAllInfo :: [String] -> Result 
getAllInfo s = Result outputLine (stats !! 0) (stats !! 1) (stats !! 2) where --(outputLine, stats) where
  s'         = take 5 $ reverse s
  stats      = fmap (read . last . words ) $ take 3 s'
  outputLine = last s'

baseDir = "examples/"

printResults :: Bool -> String -> Result -> Result -> IO () 
printResults verbose testFile (Result exOutline exResp exTurn exWait) (Result out resp turn wait) = do --testStr simStr = do
  putStrLn ("Test: " ++ testFile)
  if exOutline == out
     then if verbose then putStrLn $ "Timing diagrams match" else return ()
     else putStrLn $ "\nIncorrect result found! File: " ++ " sim result: " ++ out  ++ " expected: " ++ exOutline ++ "\n"
  statComp "Response time" exResp resp
  statComp "Turn around time" exTurn turn
  statComp "Wait time" exWait wait
  where
    statComp valStr expected actual = 
      if expected == actual
         then if verbose then putStrLn $ valStr ++ " matched expected" else return ()
         else putStrLn $ "\nIncorrect result for " ++ valStr ++ " found! Found: " ++ show actual ++ " expected: " ++ show expected ++ "\n"

main = do
  args <- getArgs
  let verbose = elem "-v" args
  if length args < 2
     then do
       putStrLn "Not enough arguments!"
       putStrLn "Usage runghc ./test_results.hs NUM_CORES SCHEME"
     else do
       let args'      = filter (/= "-v") args
           numCores   = read $ args' !! 0 :: Int
           scheme     = args' !! 1
           inputFiles = fmap (\n -> baseDir ++ "proc" ++ show n ++ ".csv" ) [1..3]
           testFiles  = fmap (\iFile -> (reverse . drop 4 . reverse $ iFile) ++ "-c" ++ show numCores ++ '-' : scheme ++ ".out") inputFiles
       -- read test files and get the revelant output
       allTestFiles <- mapM readFile testFiles
       let expectedResults = fmap (getAllInfo . lines) allTestFiles
--       let (testFinalLines, allStats) = fmap (getAllInfo . lines) allTestFiles
       -- run our code in a process
       simulatorProcs <- mapM (\f -> createProcess (proc "./simulator" ["-c", show numCores, "-s", scheme, f]) { std_out = CreatePipe } ) inputFiles

       simResults     <- mapM (\(_, Just hout, _, _) -> do
                                  outLines <- hGetContents hout
                                  return $ getAllInfo $ lines outLines) simulatorProcs
       sequence_ $ zipWith3 (printResults verbose) inputFiles expectedResults simResults
