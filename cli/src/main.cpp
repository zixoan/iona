#include <iostream>
#include <filesystem>
#include <sstream>
#include <regex>

bool IsVsCodeInstalled()
{
	std::string pathVariable = std::getenv("PATH");
	
	return pathVariable.find("Microsoft VS Code") != std::string::npos;
}

bool IsValidProjectName(const std::string& s)
{
	// Only allow alphanumeric characters, underscores and hyphens
	return std::regex_match(s, std::regex("^[a-zA-Z0-9_\-]+$"));
}

int main(int argc, char** argv)
{
	if (argc == 3)
	{
		if (strcmp(argv[1], "new") == 0)
		{
			std::string projectName = argv[2];
			if (IsValidProjectName(projectName))
			{
				std::cout << "Creating project directory for '" << projectName << std::endl;

				std::string templatesPath = std::getenv("IONA_PATH");
				templatesPath.append("/templates/vscode");

				std::filesystem::copy(templatesPath, projectName, std::filesystem::copy_options::recursive);

				std::cout << "Project directory created" << std::endl;

				if (IsVsCodeInstalled())
				{
					char answer;

					std::cout << "Open the project in Visual Studio Code? [y/n]" << std::endl;
					std::cin >> answer;

					if (answer == 'y')
					{
						std::cout << "Opening Visual Studio Code.." << std::endl;

						std::stringstream command;
						command << "code ./" << projectName << " ./" << projectName << "/Main.ion";
						system(command.str().c_str());
					}
				}

				std::cout << "Finished" << std::endl;

				return EXIT_SUCCESS;
			}
			else
			{
				std::cout << "Invalid project name. Only alphanumeric characters are allowed (including _ and -)" << std::endl;
				return EXIT_SUCCESS;
			}
		}
	}

	std::cout << "Usage: iona.exe new <ProjectName>" << std::endl;

	return EXIT_SUCCESS;
}
