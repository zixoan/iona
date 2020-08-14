/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#ifndef STANDARD_H
#define STANDARD_H

#include <random>
#include <algorithm>
#include <fstream>
#include <streambuf>
#include <iterator>
#include <filesystem>
#include <regex>

namespace Iona
{
	static std::string ToStringInternal(VariableType v)
	{
		switch (v.type)
		{
		case TokenType::String:
			return std::any_cast<std::string>(v.value);
		case TokenType::Int:
			return std::to_string(std::any_cast<int>(v.value));
		case TokenType::Float:
		{
			std::stringstream stream;
			stream << std::fixed << std::setprecision(2) << std::any_cast<float>(v.value);
			return stream.str();
		}
		case TokenType::Bool:
			return std::to_string(std::any_cast<bool>(v.value));
		case TokenType::IntArray:
		{
			std::stringstream out;
			for (auto& variable : std::any_cast<std::vector<VariableType>>(v.value))
			{
				out << std::any_cast<int>(variable.value) << ", ";
			}
			std::string s = out.str();
			return s.substr(0, s.size() - 2);
		}
		case TokenType::FloatArray:
		{
			std::stringstream out;
			for (auto& variable : std::any_cast<std::vector<VariableType>>(v.value))
			{
				out << std::any_cast<float>(variable.value) << ", ";
			}
			std::string s = out.str();
			return s.substr(0, s.size() - 2);
		}
		case TokenType::StringArray:
		{
			std::stringstream out;
			for (auto& variable : std::any_cast<std::vector<VariableType>>(v.value))
			{
				out << std::any_cast<std::string>(variable.value) << ", ";
			}
			std::string s = out.str();
			return s.substr(0, s.size() - 2);
		}
		case TokenType::BoolArray:
		{
			std::stringstream out;
			for (auto& variable : std::any_cast<std::vector<VariableType>>(v.value))
			{
				out << (std::any_cast<bool>(variable.value) ? "true" : "false") << ", ";
			}
			std::string s = out.str();
			return s.substr(0, s.size() - 2);
		}
		}

		throw std::invalid_argument("Unkown array type");
	}

	namespace Core
	{
		static void Size(std::vector<VariableType>& in, VariableType& out)
		{
			VariableType v = in[0];

			out.type = TokenType::Int;

			switch (v.type)
			{
				case TokenType::String:
					out.value = (int) std::any_cast<std::string>(v.value).size();
					break;
				case TokenType::StringArray:
				case TokenType::IntArray:
				case TokenType::BoolArray:
				case TokenType::FloatArray:
					out.value = (int) std::any_cast<std::vector<VariableType>>(v.value).size();
					break;
			}
		}

		static void Empty(std::vector<VariableType>& in, VariableType& out)
		{
			VariableType v = in[0];

			out.type = TokenType::Bool;

			switch (v.type)
			{
				case TokenType::String:
					out.value = std::any_cast<std::string>(v.value).empty();
					break;
				case TokenType::StringArray:
				case TokenType::IntArray:
				case TokenType::BoolArray:
				case TokenType::FloatArray:
					out.value = std::any_cast<std::vector<VariableType>>(v.value).empty();
					break;
			}
		}

		static void Random(std::vector<VariableType>& in, VariableType& out)
		{
			VariableType lowerBoundT = in[0];
			VariableType upperBoundT = in[1];

			int lowerBound = std::any_cast<int>(lowerBoundT.value);
			int upperBound = std::any_cast<int>(upperBoundT.value);

			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<> dis(lowerBound, upperBound);

			out.type = TokenType::Int;
			out.value = dis(gen);
		}

		static void Range(std::vector<VariableType>& in, VariableType& out)
		{
			VariableType upperBoundT = in[0];

			int upperBound = std::any_cast<int>(upperBoundT.value);

			std::vector<VariableType> values;
			for (int i = 0; i < upperBound; i++)
			{
				values.push_back({ TokenType::Int, (int) i });
			}

			out.type = TokenType::IntArray;
			out.value = std::move(values);
		}

		static void Reverse(std::vector<VariableType>& in, VariableType& out)
		{
			VariableType arrayT = in[0];

			auto array = std::any_cast<std::vector<VariableType>>(arrayT.value);

			std::reverse(array.begin(), array.end());

			out.type = arrayT.type;
			out.value = std::move(array);
		}

		static void ToString(std::vector<VariableType>& in, VariableType& out)
		{
			VariableType variableT = in[0];

			out.type = TokenType::String;
			out.value = ToStringInternal(variableT);
		}
	}

	namespace String
	{
		static void ToUpperCase(std::vector<VariableType>& in, VariableType& out)
		{
			VariableType v = in[0];

			std::string value = std::any_cast<std::string>(v.value);
			std::transform(value.begin(), value.end(), value.begin(),
				[](unsigned char c) -> unsigned char { return std::toupper(c); });

			out.type = TokenType::String;
			out.value = value;
		}

		static void ToLowerCase(std::vector<VariableType>& in, VariableType& out)
		{
			VariableType v = in[0];

			std::string value = std::any_cast<std::string>(v.value);
			std::transform(value.begin(), value.end(), value.begin(),
				[](unsigned char c) -> unsigned char { return std::tolower(c); });

			out.type = TokenType::String;
			out.value = value;
		}

		static void StartsWith(std::vector<VariableType>& in, VariableType& out)
		{
			VariableType valueT = in[0];
			VariableType prefixT = in[1];

			std::string value = std::any_cast<std::string>(valueT.value);
			std::string prefix = std::any_cast<std::string>(prefixT.value);

			out.type = TokenType::Bool;
			out.value = Helper::StartsWith(value, prefix);
		}

		static void EndsWith(std::vector<VariableType>& in, VariableType& out)
		{
			VariableType valueT = in[0];
			VariableType suffixT = in[1];

			std::string value = std::any_cast<std::string>(valueT.value);
			std::string suffix = std::any_cast<std::string>(suffixT.value);

			out.type = TokenType::Bool;
			out.value = Helper::EndsWith(value, suffix);
		}

		static void Contains(std::vector<VariableType>& in, VariableType& out)
		{
			VariableType haystackT = in[0];
			VariableType needleT = in[1];

			std::string haystack = std::any_cast<std::string>(haystackT.value);
			std::string needle = std::any_cast<std::string>(needleT.value);

			out.type = TokenType::Bool;
			out.value = haystack.find(needle) != std::string::npos;
		}

		static void Split(std::vector<VariableType>& in, VariableType& out)
		{
			VariableType valueT = in[0];
			VariableType delimiterT = in[1];

			std::string value = std::any_cast<std::string>(valueT.value);
			std::string delimiter = std::any_cast<std::string>(delimiterT.value);

			std::vector<VariableType> values;
			size_t start;
			size_t end = 0;

			while ((start = value.find_first_not_of(delimiter, end)) != std::string::npos)
			{
				end = value.find(delimiter, start);
				values.push_back({ TokenType::String, value.substr(start, end - start) });
			}

			out.type = TokenType::StringArray;
			out.value = std::move(values);
		}

		static void Trim(std::vector<VariableType>& in, VariableType& out)
		{
			VariableType valueT = in[0];

			std::string value = std::any_cast<std::string>(valueT.value);

			value.erase(value.begin(), std::find_if(value.begin(), value.end(), [](int ch) {
				return !std::isspace(ch);
			}));

			value.erase(std::find_if(value.rbegin(), value.rend(), [](int ch) {
				return !std::isspace(ch);
				}).base(), value.end());

			out.type = TokenType::String;
			out.value = std::move(value);
		}
	}

	namespace File
	{
		namespace fs = std::filesystem;

		static void FileExists(std::vector<VariableType>& in, VariableType& out)
		{
			VariableType v = in[0];

			std::string path = std::any_cast<std::string>(v.value);

			out.type = TokenType::Bool;
			out.value = fs::exists(path);
		}

		static void FileRead(std::vector<VariableType>& in, VariableType& out)
		{
			VariableType v = in[0];

			std::string path = std::any_cast<std::string>(v.value);

			std::ifstream file{ path };
			
			out.type = TokenType::String;
			out.value = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
		}

		static void FileWrite(std::vector<VariableType>& in, VariableType& out)
		{
			VariableType pathT = in[0];
			VariableType dataT = in[1];

			std::string path = std::any_cast<std::string>(pathT.value);
			std::string data = std::any_cast<std::string>(dataT.value);

			std::ofstream o{ path, std::ofstream::out };
			o << data;
			
			out.type = TokenType::Bool;
			out.value = !o.bad();
		}

		static void FileCopy(std::vector<VariableType>& in, VariableType& out)
		{
			VariableType srcPathT = in[0];
			VariableType destPathT = in[1];

			std::string srcPath = std::any_cast<std::string>(srcPathT.value);
			std::string destPath = std::any_cast<std::string>(destPathT.value);

			std::error_code error;
			std::filesystem::copy(srcPath, destPath, fs::copy_options::recursive, error);

			out.type = TokenType::Bool;
			out.value = !error;
		}

		static void FileReadLines(std::vector<VariableType>& in, VariableType& out)
		{
			VariableType v = in[0];

			std::string path = std::any_cast<std::string>(v.value);

			std::vector<VariableType> lines;

			std::ifstream file{ path };
			std::string line;
			while (std::getline(file, line))
			{
				lines.push_back({ TokenType::String, line });
			}
			file.close();

			out.type = TokenType::StringArray;
			out.value = lines;
		}

		static void FileWriteLines(std::vector<VariableType>& in, VariableType& out)
		{
			VariableType pathT = in[0];
			VariableType dataT = in[1];
			VariableType appendT = in[2];

			std::string path = std::any_cast<std::string>(pathT.value);
			std::vector<VariableType> data = std::any_cast<std::vector<VariableType>>(dataT.value);
			bool append = std::any_cast<bool>(appendT.value);

			std::ofstream file{ path, append ? std::ios_base::app : std::ios_base::trunc };
			for (const auto& value : data)
			{
				file << std::any_cast<std::string>(value.value) << std::endl;
			}
			file.close();

			out.type = TokenType::Bool;
			out.value = !file.bad();
		}

		static void FileList(std::vector<VariableType>& in, VariableType& out)
		{
			VariableType pathT = in[0];
			VariableType regexStringT = in[1];

			std::string path = std::any_cast<std::string>(pathT.value);
			std::string regexString = std::any_cast<std::string>(regexStringT.value);
			
			std::vector<VariableType> files;

			try
			{
				if (regexString.empty())
				{
					regexString = ".*";
				}

				std::regex regex(regexString);

				for (auto& p : std::filesystem::recursive_directory_iterator(path))
				{
					if (std::regex_match(p.path().string(), regex))
					{
						files.push_back(VariableType{ TokenType::String, p.path().string() });
					}
				}
			}
			catch (const std::regex_error&)
			{
				Exit("FileFind: String '%s' is not a valid regex", regexString.c_str());
			}
			catch (const std::exception&)
			{
				// TODO: Language runtime error handling
			}

			out.type = TokenType::StringArray;
			out.value = files;
		}
	}

	namespace Console
	{
		static void WriteLine(std::vector<VariableType>& in, VariableType& out)
		{
			VariableType v = in[0];

			printf("%s\n", ToStringInternal(v).c_str());
		}

		static void ReadLine(std::vector<VariableType>& in, VariableType& out)
		{
			std::string input;
			std::getline(std::cin, input);

			out.type = TokenType::String;
			out.value = input;
		}

		static void ReadInt(std::vector<VariableType>& in, VariableType& out)
		{
			int read;
			std::cin >> read;

			out.type = TokenType::Int;
			out.value = read;
		}

		static void ReadFloat(std::vector<VariableType>& in, VariableType& out)
		{
			float read;
			std::cin >> read;

			out.type = TokenType::Float;
			out.value = read;
		}
	}

	namespace Math
	{
		static void Min(std::vector<VariableType>& in, VariableType& out)
		{
			VariableType valueOneT = in[0];
			VariableType valueTwoT = in[1];

			if (valueOneT.type == TokenType::Int && valueTwoT.type == TokenType::Int)
			{
				int valueOne = std::any_cast<int>(valueOneT.value);
				int valueTwo = std::any_cast<int>(valueTwoT.value);

				out.type = TokenType::Int;
				out.value = std::min(valueOne, valueTwo);
			}
			else if (valueOneT.type == TokenType::Float && valueTwoT.type == TokenType::Float)
			{
				float valueOne = std::any_cast<float>(valueOneT.value);
				float valueTwo = std::any_cast<float>(valueTwoT.value);

				out.type = TokenType::Float;
				out.value = std::min(valueOne, valueTwo);
			}
		}

		static void Max(std::vector<VariableType>& in, VariableType& out)
		{
			VariableType valueOneT = in[0];
			VariableType valueTwoT = in[1];

			if (valueOneT.type == TokenType::Int && valueTwoT.type == TokenType::Int)
			{
				int valueOne = std::any_cast<int>(valueOneT.value);
				int valueTwo = std::any_cast<int>(valueTwoT.value);

				out.type = TokenType::Int;
				out.value = std::max(valueOne, valueTwo);
			}
			else if (valueOneT.type == TokenType::Float && valueTwoT.type == TokenType::Float)
			{
				float valueOne = std::any_cast<float>(valueOneT.value);
				float valueTwo = std::any_cast<float>(valueTwoT.value);

				out.type = TokenType::Float;
				out.value = std::max(valueOne, valueTwo);
			}
		}
	}
}

#endif