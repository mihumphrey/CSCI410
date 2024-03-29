# This script takes a project (01|02|03|...|13) and runs all tests associated with that project
# Usage: runTests.sh <project name>. e.g: runTests.sh 01
# Shows all correct and incorrect hdl files 
# Prints all errors to errors.log


# Declare all color variables that will be used
RED='\033[0;31m'
NC='\033[0m'
GREEN='\033[0;32m'

PROJECT=$1                                                  # Stores the project given to the user
WD=$(pwd)                                                   # Stores the current working directory to return to when the script finishes
SUCCESS="End of script - Comparison ended successfully"     # Stores the success line when a test is successul
CORRECT=0                                                   # Initializes the number of passing tests to zero

# Changes directory to wherever the project is located (CHANGE THIS TO WHEREVER YOU HAVE YOUR PROJECT STORED)
cd /u/au/sw/michaelhumphrey/GitHubRepositories/CSCI410/projects/$PROJECT
cd ~/GithubRepositories/CSCI410/projects/$PROJECT

COUNT=$(ls *.tst | wc -l)                                   # Stores the count of all .tst files in the given directory

rm -f errors.log

# Prints intro
echo
echo "Running all tests for project $PROJECT. To view all errors, please view $(pwd)/errors.log"
echo

# Runs each test, printing it green if it passes, and red otherwise
for i in $(ls *tst); 
do
    # Save the output to OUTPUT, redirecting all stderr to a log file
	OUTPUT=$(HardwareSimulator.sh $i 2>> errors.log);

    # If output matches, print filename in green and increment CORRECT variable
    if [[ $OUTPUT = $SUCCESS ]]
    then
        CORRECT=$((CORRECT+1))
        echo -e "${GREEN}$i"
    # If output does not match, print filename in red 
    else
        echo -e "${RED}$i"
    fi
done	

# Print the correct/total tests to console
echo -e "${NC}"
echo "$CORRECT/$COUNT"

# If all correct and README exists, create a zip file containing README and all .hdl files
if [[ $CORRECT = $COUNT ]]
then
    RE="YES"
    
    # If README does not exist, ask if user wants to create it
    if [ ! -f README ]
    then 
        # Ask if user wants to create README file and parse input
        echo "No README was found. Would you like to create one now?"
        read RESPONSE
        if [[ $RESPONSE = "Y" ]] || [[ $RESPONSE = "y" ]] || [[ $RESPONSE = "yes" ]] || [[ $RESPONSE = "YES" ]]
        then
            # Edit the README file
            vim README
        else
            # README does not exist, do not create zip file
            RE="NO"
        fi
    fi

    # Remove any previous zip files
    if [ -f michaelhumphrey.zip ]
    then
        rm -f michaelhumphrey.zip
    fi
    if [[ $RE = "YES" ]] && [[ -f README ]]
    # Make temporary src directory with all hdl files then zip all hdl files as well as the README
    then
        echo "Creating zip file 'michaelhumphrey.zip' containing README and all .hdl files"
        rm -rf src
        mkdir src
        cp *.hdl src
        zip -r michaelhumphrey.zip src README >> /dev/null
        rm -rf src
    fi
fi

# Change directory to wherever user was previously
cd $WD

