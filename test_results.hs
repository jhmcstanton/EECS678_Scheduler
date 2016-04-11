import System.Environment (getArgs)
import System.Process (createProcess, proc, StdStream( CreatePipe ), CreateProcess( .. ) )
import GHC.IO.Handle (hGetContents)
import Text.Printf

getOutLine :: [String] -> String
getOutLine = (!! 4) . reverse

baseDir = "examples/"

printResults :: String -> String -> String -> IO ()
printResults testFile testStr simStr = do
  if testStr == simStr
     then putStrLn $ testFile ++ " performed as expected"
     else putStrLn $ "Incorrect result found! File: " ++ testFile ++ " sim result: " ++ simStr ++ " expected: " ++ testStr

main = do
  putStrLn "Currently does not test calculations, just timing diagrams!"
  args <- getArgs
  if length args < 2
     then do
       putStrLn "Not enough arguments!"
       putStrLn "Usage runghc ./test_results.hs NUM_CORES SCHEME"
     else do
       let numCores   = read $ args !! 0 :: Int
           scheme     = args !! 1
           inputFiles = fmap (\n -> baseDir ++ "proc" ++ show n ++ ".csv" ) [1..3]
           testFiles  = fmap (\iFile -> (reverse . drop 4 . reverse $ iFile) ++ "-c" ++ show numCores ++ '-' : scheme ++ ".out") inputFiles
       -- read test files and get the revelant output
       allTestFiles <- mapM readFile testFiles
       let testFinalLines = fmap (getOutLine . lines) allTestFiles
       -- run our code in a process
       simulatorProcs <- mapM (\f -> createProcess (proc "./simulator" ["-c", show numCores, "-s", scheme, f]) { std_out = CreatePipe } ) inputFiles

       simFinalLines <- mapM (\(_, Just hout, _, _) -> do
                                  outLines <- hGetContents hout
                                  return $ getOutLine $ lines outLines) simulatorProcs
       sequence_ $ zipWith3 printResults inputFiles testFinalLines simFinalLines
