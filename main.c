//imort all required libraries
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <conio.h>

//patient struct
struct patient
{
	int ppsNumber;
	char firstName[20];
	char secondName[20];
	char yearBorn[20];
	char gender;
	char emailAddress[20];
	char nextOfKinName[20];
	char lastAppoinment[20];
	float weight;
	float height;
	char allergy[20];
	int smoking;
	int drinking;
	int excercise;
	struct patient* NEXT;
};

//struct for login
typedef struct {
	char username[20];
	char password[20];
}userLogin;

//Login user
void readLogin(userLogin* login);
int validate_user(userLogin* login);

//Add a patient to the database
void AddPatient(struct patient** top);

//Display either the entire database of patients or one patient in specific
void DisplayAllPatients(struct patient* top);
void DisplayOnePatient(struct patient* top);

//update a patient
void updatePatient(struct patient* top);

//Delete Patients
void DeletePatientAtStart(struct patient** headptr);
void DeletePatientAtEnd(struct patient* headptr);
void DeletePatientAtPos(struct patient** headptr);

//Read in the patients.txt file
int initialFileRead(struct patient** headPtr, FILE * infile);
void inputFileRead(struct patient* head, FILE* infile);
void outputPatientsFile(struct patient* headptr);

//calculate the statistics
void calculateStatistics(struct patient* top, int num);

//Save to Report.txt
void SaveToReport(struct patient* headptr);

//check if the pps number entered is unique
int searchList(struct patient* top, int searchNumber);

//output patient in order of their last appointment
void ListAppointment(struct  patient* top);

void main() 
{
	//declare variables and 
	struct patient* patients = NULL;
	int menu, option;
	int fileCount = 0;
	int i;
	int count = 0;

	//count patients for inputting from file
	int countPlayers;

	//used to validate user
	userLogin* login;
	login = (userLogin*)malloc(sizeof(userLogin) * 3);
	FILE* patientsFile;

	//read the login file
	readLogin(login);

	// read in previous data from file
	patientsFile = fopen("patients.txt", "r");

	//if the patient file is empty
	if (patientsFile == NULL) 
	{
		printf("Patients file does not exit - please create it first!\n\n");
	}
	else 
	{
		//if patient file is not empty read in the file
		
		//increment depending on the file input
		count = initialFileRead(&patients, patientsFile);
		//printf("Debug: %d",count);

		//output the patients to the database
		for (i = 1; i < count; i++) 
		{
			inputFileRead(patients, patientsFile);
		}

		printf("The patients file has been read in and can be displayed after logging in!\n\n");

		//close the file
		fclose(patientsFile);
	}
	
	//validate the user and if login is correct then output the menu 
	if (validate_user(login) == 1)
	{
		//menu options
		printf("Please choose one of the following options\n");
		printf("============================================\n");
		printf("Press 1 to add a patient\nPress 2 to display all patient detials\nPress 3 to display a specific patient\n");
		printf("Press 4 to update a patient\nPress 5 to delete a patient\nPress 6 to generate statistics for all the patients\n");
		printf("Press 7 to print all patients to a report file\nPress 8 to list all the patients of the following countries in order of their last appointment\nPress -1 to exit\n");
		scanf("%d", &menu);
		printf("============================================\n");
		printf("\n");

		// while loop to run through the menu options
		while (menu != -1)
		{
			if (menu == 1)
			{
				printf("Adding a new patient\n");
				AddPatient(&patients);
			}// add a patient
			else if (menu == 2)
			{
				DisplayAllPatients(patients);
			}//Display all patients
			else if (menu == 3)
			{
				DisplayOnePatient(patients);
			}//display one patient
			else if (menu == 4)
			{
				updatePatient(patients);
			}//update a patient depending on their name and pps
			else if (menu == 5)
			{
				//delete depending on users preference
				printf("Please enter 1 to delete a patient at the start\n");
				printf("Please enter 2 to delete a patient at the end\n");
				printf("Please enter 3 to delete a patient at a position: ");
				scanf("%d", &option);

				if (option == 1)
				{
					DeletePatientAtStart(&patients);
				}//delete at start of database
				else if (option == 2)
				{
					DeletePatientAtEnd(patients);
				}//delete at the end of the database
				else if (option == 3)
				{
					DeletePatientAtPos(&patients);
				}//delete at a position
				else
				{
					printf("wrong option\n");
				}
			}
			else if (menu == 6)
			{
				calculateStatistics(patients, 1);
			}//calculate the statistisc/bmi depending on the two option
			else if (menu == 7)
			{
				SaveToReport(patients);
			}//save the patients details to a report file
			else if (menu == 8)
			{
				ListAppointment(patients);
			}//list the patients in order of their last appointment
			else
			{
				printf("Please try again\n\n");
			}

			//Menu options
			printf("\n");
			printf("Please choose one of the following options\n");
			printf("============================================\n");
			printf("Press 1 to add a patient\nPress 2 to display all patient detials\nPress 3 to display a specific patient\n");
			printf("Press 4 to update a patient\nPress 5 to delete a patient\nPress 6 to generate statistics for all the patients\n");
			printf("Press 7 to print all patients to a report file\nPress 8 to list all the patients of the following countries in order of their last appointment\nPress -1 to exit\n");
			scanf("%d", &menu);
			printf("============================================\n");
			printf("\n");
		}// while
	}

	//output the patients to the patients file
	outputPatientsFile(patients);


}// main

//read in the login file
void readLogin(userLogin* login)
{
	FILE* file;
	int i = 0;
	int j;
	//open the login file
	file = fopen("login.txt", "r");

	//if its empty then return and close program
	if (file == NULL)
	{
		printf("File doesnt exist!");
		return;
	}
	else {
		//else save the file to the login struct
		for(j = 0; j < 3; j++)
		{
			fscanf(file, "%s %s", login[i].username, login[i].password);
			i++;
		} 
	}
}

//confirm the user matches the details of what is in the login file
int validate_user(userLogin* login)
{
	char name[20], password[20], c;
	int index = 0;
	int i;
	int p = 0;

	//ask for username and password
	printf("Please enter your username: ");
	scanf("%s", &name);
	printf("Please enter your password: ");

	//when the user enters the password, then replace it with *
	//https://www.geeksforgeeks.org/print-in-place-of-characters-for-reading-passwords-in-c/
	do {
		password[p] = getch();
		if (password[p] != '\r') {
			printf("*");
		}
		p++;
	} while (password[p - 1] != '\r');
	password[p - 1] = '\0';
	getch();

	//run the for loop 3 times due to three logins and passwords
	for (i = 0; i < 3; i++) 
	{
		//compare the username entered with the one saved and print out the message else login failed
		if (strcmp(name, (login + i)->username) == 0 && strcmp(password, (login + i)->password) == 0)
		{
			printf("\nYou have successfully logged in!\n\n");
			return 1;
		}
	}

	printf("\nThat is not a valid username or password!\n\n");
	return 0;
}

//add a patient to the database
void AddPatient(struct patient** top)
{
	struct patient* newPatient = *top;
	int ppsNumber;

	//get the size of struct
	newPatient = (struct patient*)malloc(sizeof(struct patient));

	//ask for pps and if its unique ask for other details
	printf("Please enter patients PPS Number: ");
	scanf("%d", &newPatient->ppsNumber);

	ppsNumber = newPatient->ppsNumber;

	//check if pps is unique
	if (searchList(*top, ppsNumber) == 1)
	{
		printf("\nSorry the PPS number already exists.\n");
		return;
	}

	printf("Please enter patients First Name: ");
	scanf("%s", &newPatient->firstName);
	printf("Please enter patients Last Name: ");
	scanf("%s", &newPatient->secondName);
	printf("Please enter patients Date of Birth (dd/mm/yy): ");
	scanf("%s", &newPatient->yearBorn);
	printf("Please enter patients Gender (m/f): ");
	scanf(" %s", &newPatient->gender);
	printf("Please enter patients E-mail Address: ");
	scanf("%s", &newPatient->emailAddress);
	printf("Please enter patients next of Kin name: ");
	scanf("%s", &newPatient->nextOfKinName);
	printf("Please enter the last appointment  (ddmmyy): ");
	scanf("%s", &newPatient->lastAppoinment);
	printf("Please enter patients weight in KG: ");
	scanf("%f", &newPatient->weight);
	printf("Please enter patients height in Metres: ");
	scanf("%f", &newPatient->height);
	printf("Does the patient have any allergies (yes/no): ");
	scanf("%s", &newPatient->allergy);
	printf("How many cigarettes does the patient smoke everyday (1. None  2.Less than 10  3.More than 10): ");
	scanf("%d", &newPatient->smoking);
	printf("How much alcohol does the patient drink everyweek (1. None  2.Less than 10 units  3.More than 10 units): ");
	scanf("%d", &newPatient->drinking);
	printf("How often does the patient excercise (1. Never  2.Less than 2 per week  3.More than 2 a per week): ");
	scanf("%d", &newPatient->excercise);

	newPatient->NEXT = *top;
	*top = newPatient;

}

//functionto check if the pps number is unique
int searchList(struct patient* top, int searchNumber)
{

	int found = 0;
	struct patient* temp = top;

	while (temp != NULL)
	{
		if (temp->ppsNumber == searchNumber)
		{
			found = 1;
			return found;
		}

		temp = temp->NEXT;
	}

	return found;
}

//display all patients
void DisplayAllPatients(struct patient* top)
{
	struct patient* temp = top;
	//if empty then return else print out all details
	if (temp == NULL)
	{
		printf("Sorry the database is empty\n");
	}

	while (temp != NULL)
	{
		printf("Patient's PPS Number: %d\n", temp->ppsNumber);
		printf("Patient's First Name: %s\n", temp->firstName);
		printf("Patient's Second Name: %s\n", temp->secondName);
		printf("Patient's Date of Birth (dd/mm/yy): %s\n", temp->yearBorn);
		printf("Patient's Gender: %c\n", temp->gender);
		printf("Patient's Patient's E-mail: %s\n", temp->emailAddress);
		printf("Patient's Next of Kin Name: %s\n", temp->nextOfKinName);
		printf("Patient's last appointment: %s\n", top->lastAppoinment);
		printf("Patient's Weight: %.2f\n", temp->weight);
		printf("Patient's Height %.2f\n", temp->height);
		printf("Patient's Allergies: %s\n", temp->allergy);
		printf("Patient's Smoking habits: %d\n", temp->smoking);
		printf("Patient's Alcohol habits: %d\n", temp->drinking);
		printf("Patient's Excercise habits: %d\n\n", temp->excercise);

		temp = temp->NEXT;

	}// while

}// DisplayPatient

//display one patient depending on pps or name entered
void DisplayOnePatient(struct patient* top)
{
	int search;
	int pps = 1;
	char name[20];
	printf("Please press 1 to search by PPS number or 2 to search by name: ");
	scanf("%d", &search);

	if (search == 1)
	{
		printf("Please enter the pps number of the patient: ");
		scanf("%d", &pps);
		printf("\n");
	}//search by pps
	else if (search == 2)
	{
		printf("Please enter the first name of the patient: ");
		scanf("%s", &name);
		printf("\n");
	}//search by name
	else
	{
		printf("Wrong option\n");
	}

	//if not empty
	while (top != NULL)
	{
		//if pps or name match then print out the details
		if (top->ppsNumber == pps || strcmp(top->firstName, name) == 0)
		{
			printf("Patient's PPS Number: %d\n", top->ppsNumber);
			printf("Patient's First Name: %s\n", top->firstName);
			printf("Patient's Second Name: %s\n", top->secondName);
			printf("Patient's Date of Birth (dd/mm/yy): %d\n", top->yearBorn);
			printf("Patient's Gender: %c\n", top->gender);
			printf("Patient's E-mail: %s\n", top->emailAddress);
			printf("Patient's Next of Kin Name: %s\n", top->nextOfKinName);
			printf("Patient's last appointment: %s\n", top->lastAppoinment);
			printf("Patient's Weight: %.2f\n", top->weight);
			printf("Patient's Height %.2f\n", top->height);
			printf("Patient's Allergies: %s\n", top->allergy);
			printf("Patient's Smoking habits: %d\n", top->smoking);
			printf("Patient's Alcohol habits: %d\n", top->drinking);
			printf("Patient's Excercise habits: %d\n", top->excercise);
			return;
		}
		top = top->NEXT;
	}
	//incorrect pps or name
	printf("Patient with this PPS number or name was not found\n");

}// DisplayPatient

//update a patient depending on pps or name entered
void updatePatient(struct patient* headptr)
{
	int search;
	int pps = 1;
	char name[20];
	printf("Please press 1 to search by PPS number or 2 to search by name: ");
	scanf("%d", &search);

	if (search == 1)
	{
		printf("Please enter the pps number of the patient: ");
		scanf("%d", &pps);
	}
	else if (search == 2)
	{
		printf("Please enter the first name of the patient: ");
		scanf("%s", &name);
	}
	else
	{
		printf("Wrong option\n");
		return;
	}

	struct patient *newNode;
	newNode = (struct node*)malloc(sizeof(struct patient));

	newNode = headptr;

	while (newNode != NULL)
	{
		//if name or pps match allow user to update the patient
		if (newNode->ppsNumber == pps || strcmp(newNode->firstName, name) == 0)
		{
			printf("Please enter patients new E-mail Address: ");
			scanf("%s", &newNode->emailAddress);
			printf("Please enter patients new next of Kin name: ");
			scanf("%s", &newNode->nextOfKinName);
			printf("Please enter the last appointment  (dd/mm/yy): ");
			scanf("%s", &newNode->lastAppoinment);
			printf("Please enter patients new weight: ");
			scanf("%f", &newNode->weight);
			printf("Please enter patients new height: ");
			scanf("%f", &newNode->height);
			printf("Does the patient still have any allergies (yes/no): ");
			scanf("%s", &newNode->allergy);
			printf("How many cigarettes does the patient smoke everyday (1. None  2.Less than 10  3.More than 10): ");
			scanf("%d", &newNode->smoking);
			printf("How much alcohol does the patient drink everyweek (1. None  2.Less than 10 units  3.More than 10 units): ");
			scanf("%d", &newNode->drinking);
			printf("How often does the patient excercise (1. Never  2.Less than 2 per week  3.More than 2 a per week): ");
			scanf("%d", &newNode->excercise);
			return;
		}

		newNode = newNode->NEXT;

	}

	printf("The pps or name you have entered is incorrect\n");

}

//delete a patient at the start
void DeletePatientAtStart(struct patient** headptr)
{
	struct patient *temp;

	temp = *headptr;
	*headptr = temp->NEXT;

	free(temp);

	printf("Patient at start deleted!\n");

}

//delete a patient at the end
void DeletePatientAtEnd(struct patient* headptr)
{
	struct patient* temp;
	struct patient* prev_temp;

	temp = headptr;
	//if database is not empty then enter the while loop
	while (temp->NEXT != NULL)
	{
		prev_temp = temp;
		temp = temp->NEXT;
	}
	//set the last temp equal to null
	prev_temp->NEXT = NULL;
	free(temp);

	printf("Patient at end deleted!\n");
}

//delete at a position
//https://www.geeksforgeeks.org/linked-list-set-3-deleting-node/
void DeletePatientAtPos(struct patient** headptr)
{
	int pps = 1;

	printf("Please enter the pps number of the patient: ");
	scanf("%d", &pps);

	// Store head node 
	struct patient* temp = *headptr, *prev;

	// If head node itself holds the key to be deleted 
	if (temp != NULL && temp->ppsNumber == pps)
	{
		*headptr = temp->NEXT;   // Changed head 
		free(temp);               // free old head 
		printf("Patient Deleted\n");
		return;
	}

	// Search for the key to be deleted, keep track of the 
	// previous node as we need to change 'prev->next' 
	while (temp != NULL && temp->ppsNumber != pps)
	{
		prev = temp;
		temp = temp->NEXT;
	}

	// If key was not present in linked list 
	if (temp == NULL) return;

	// Unlink the node from linked list 
	prev->NEXT = temp->NEXT;

	free(temp);  // Free memory 

	printf("Patient at position deleted!\n");
}

//output the details to a patients file
void outputPatientsFile(struct patient* headptr)
{
	struct patient *temp;
	temp = (struct node*)malloc(sizeof(struct patient));
	temp = headptr;
	FILE * outfile;

	//open the file
	outfile = fopen("patients.txt", "w+");
	//fprintf(outfile, "%d\n", length(headptr));

	//if database not empty
	while (temp != NULL) 
	{
		//output to patients file
		fprintf(outfile, "%d\n", temp->ppsNumber);
		fprintf(outfile, "%s\n", temp->firstName);
		fprintf(outfile, "%s\n", temp->secondName);
		fprintf(outfile, "%s\n", temp->yearBorn);
		fprintf(outfile,"%c\n", temp->gender);
		fprintf(outfile, "%s\n", temp->emailAddress);
		fprintf(outfile, "%s\n", temp->nextOfKinName);
		fprintf(outfile, "%s\n", temp->lastAppoinment);
		fprintf(outfile, "%.2f\n", temp->weight);
		fprintf(outfile, "%.2f\n", temp->height);
		fprintf(outfile, "%s\n", temp->allergy);
		fprintf(outfile, "%d\n", temp->smoking);
		fprintf(outfile, "%d\n", temp->drinking);
		fprintf(outfile, "%d\n", temp->excercise);
		temp = temp->NEXT;
	}
	fclose(outfile);
}

//read in the patients file - the initial read to count the  number of patients
int initialFileRead(struct patient** headPtr, FILE * infile) 
{
	struct patient* newNode;
	newNode = (struct patient*)malloc(sizeof(struct patient));
	int patientsCount = 0;

	fscanf(infile, "%d\n", &patientsCount);

	if (patientsCount != 0) 
	{
		//output to the struct
		fscanf(infile, "%d %s %s %s %s %s %s %s %f %f %s %d %d %d",
			&newNode->ppsNumber, &newNode->firstName, &newNode->secondName, &newNode->yearBorn,
			&newNode->gender, &newNode->emailAddress, &newNode->nextOfKinName, &newNode->lastAppoinment,
			&newNode->weight, &newNode->height, &newNode->allergy, &newNode->smoking, &newNode->drinking,
			&newNode->excercise);

		newNode->NEXT = *headPtr;
		*headPtr = newNode;
	}
	//return the patients count
	return patientsCount;
}

//read the patients file properly
void inputFileRead(struct patient* head, FILE* infile) 
{
	struct patient *temp;
	temp = (struct patient*)malloc(sizeof(struct patient));
	temp = head;

	struct patient* newNode;
	newNode = (struct patient*)malloc(sizeof(struct patient));

	while (temp->NEXT != NULL) 
	{
		temp = temp->NEXT;
	}
	//output the patients file to the struct
	fscanf(infile, "%d %s %s %s %s %s %s %s %f %f %s %d %d %d",
		&newNode->ppsNumber, &newNode->firstName, &newNode->secondName, &newNode->yearBorn,
		&newNode->gender, &newNode->emailAddress, &newNode->nextOfKinName, &newNode->lastAppoinment,
		&newNode->weight, &newNode->height, &newNode->allergy, &newNode->smoking, &newNode->drinking,
		&newNode->excercise);

	newNode->NEXT = NULL;
	temp->NEXT = newNode;
}

// calculate the statistics based on the smoking and exercise habits
void calculateStatistics(struct patient* top, int num)
{

	int option;
	int cigerettes;
	int exercise;
	float calculateBmi;
	float height;
	float weight;

	int bmi1 = 0;
	int bmi2 = 0;
	int bmi3 = 0;
	int bmi4 = 0;

	//save to the report file
	struct patient *temp;
	temp = (struct node*)malloc(sizeof(struct patient));
	temp = top;
	FILE * reportFile;

	reportFile = fopen("report.txt", "a");

	//if num is 1 then output the statistics to the user
	if (num == 1)
	{
		//ask what option they would like to use
		printf("Please press 1 to display bmi based on smoking!\n");
		printf("Please press 2 to display bmi based on exercise!\n");
		scanf("%d", &option);

		if (option == 1)
		{
			//calculate depending on cigerette habits
			printf("How many cigerettes do you smoke everyday?\n");
			printf("1) Never\n");
			printf("2) Less then 10\n");
			printf("3) Less more 10\n");
			scanf("%d", &cigerettes);

			while (top != NULL)
			{
				if (top->smoking == cigerettes)
				{
					//get the height and wight to calculate the bmi
					height = top->height;
					weight = top->weight;

					//calculate the bmi
					calculateBmi = weight / (height*height);

					//depending on the bmi value increment a variable
					if (calculateBmi > 0 && calculateBmi < 18.5)
					{
						bmi1++;
					}
					else if (calculateBmi >= 18.5 && calculateBmi < 25)
					{
						bmi2++;
					}

					else if (calculateBmi >= 25 && calculateBmi <= 30)
					{
						bmi3++;
					}
					else if (calculateBmi > 30)
					{
						bmi4++;
					}

				}

				top = top->NEXT;

			}
			//print out the bmi information
			printf("Patients with a BMI of less than 18.5: %d\n", bmi1);
			printf("Patients with a BMI of less than 25: %d\n", bmi2);
			printf("Patients with a BMI of less than 30: %d\n", bmi3);
			printf("Patients with a BMI of greater than 30: %d\n", bmi4);
		}
		else if (option == 2)
		{
			//same as cigerettes but for exercise
			printf("How many times a week do you exercise?\n");
			scanf("%d", &exercise);

			while (top != NULL)
			{
				if (top->excercise == exercise)
				{
					height = top->height;
					weight = top->weight;

					calculateBmi = weight / (height*height);
				}

				top = top->NEXT;

				if (calculateBmi > 0 && calculateBmi < 18.5)
				{
					bmi1++;
				}
				else if (calculateBmi >= 18.5 && calculateBmi < 25)
				{
					bmi2++;
				}

				else if (calculateBmi >= 25 && calculateBmi <= 30)
				{
					bmi3++;
				}
				else if (calculateBmi > 30)
				{
					bmi4++;
				}

			}

			printf("Patients with a BMI of less than 18.5: %d\n", bmi1);
			printf("Patients with a BMI of less than 25: %d\n", bmi2);
			printf("Patients with a BMI of less than 30: %d\n", bmi3);
			printf("Patients with a BMI of greater than 30: %d\n", bmi4);
		}
		else
		{
			printf("Wrong option\n");
		}
	}
	else if(num == 2)//option 2 is to print to the report file
	{
		while (top != NULL)
		{
			if (top->smoking == cigerettes)
			{
				//get the height and wight to calculate the bmi
				height = top->height;
				weight = top->weight;

				//calculate the bmi
				calculateBmi = weight / (height*height);

				//depending on the bmi value increment a variable
				if (calculateBmi > 0 && calculateBmi < 18.5)
				{
					bmi1++;
				}
				else if (calculateBmi >= 18.5 && calculateBmi < 25)
				{
					bmi2++;
				}

				else if (calculateBmi >= 25 && calculateBmi <= 30)
				{
					bmi3++;
				}
				else if (calculateBmi > 30)
				{
					bmi4++;
				}

			}

			top = top->NEXT;

		}

			while (top != NULL)
			{
				if (top->excercise == exercise)
				{
					height = top->height;
					weight = top->weight;

					calculateBmi = weight / (height*height);
				}

				top = top->NEXT;

				if (calculateBmi > 0 && calculateBmi < 18.5)
				{
					bmi1++;
				}
				else if (calculateBmi >= 18.5 && calculateBmi < 25)
				{
					bmi2++;
				}

				else if (calculateBmi >= 25 && calculateBmi <= 30)
				{
					bmi3++;
				}
				else if (calculateBmi > 30)
				{
					bmi4++;
				}

			}
			fprintf(reportFile, "Patients with a BMI of less than 18.5: %d\n", bmi1);
			fprintf(reportFile, "Patients with a BMI of less than 25: %d\n", bmi2);
			fprintf(reportFile, "Patients with a BMI of less than 30: %d\n", bmi3);
			fprintf(reportFile, "Patients with a BMI of greater than 30: %d\n", bmi4);
		}

}

//save the results to the report file 
void SaveToReport(struct patient* headptr)
{
	//save to the report file
	struct patient *temp;
	temp = (struct node*)malloc(sizeof(struct patient));
	temp = headptr;
	FILE * reportFile;

	reportFile = fopen("report.txt", "w+");

	while (temp != NULL)
	{
		fprintf(reportFile, "%d\n", temp->ppsNumber);
		fprintf(reportFile, "%s\n", temp->firstName);
		fprintf(reportFile, "%s\n", temp->secondName);
		fprintf(reportFile, "%s\n", temp->yearBorn);
		fprintf(reportFile, "%c\n", temp->gender);
		fprintf(reportFile, "%s\n", temp->emailAddress);
		fprintf(reportFile, "%s\n", temp->nextOfKinName);
		fprintf(reportFile, "%s\n", temp->lastAppoinment);
		fprintf(reportFile, "%.2f\n", temp->weight);
		fprintf(reportFile, "%.2f\n", temp->height);
		fprintf(reportFile, "%s\n", temp->allergy);
		fprintf(reportFile, "%d\n", temp->smoking);
		fprintf(reportFile, "%d\n", temp->drinking);
		fprintf(reportFile, "%d\n", temp->excercise);
		temp = temp->NEXT;
	}
	//close the file
	fclose(reportFile);

	//output the bmi statistics to the report file
	calculateStatistics(temp, 2);
}

//output all patients in order of last appointment
void ListAppointment(struct  patient* top)
{
	struct patient* temp = top;
	//if empty then return else print out all details
	if (temp == NULL)
	{
		printf("Sorry the database is empty\n");
	}

	while (temp != NULL)
	{
		printf("Patient's last appointment: %s\n", top->lastAppoinment);

		temp = temp->NEXT;

	}// while
}
