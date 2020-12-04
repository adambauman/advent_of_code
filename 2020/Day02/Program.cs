using System;
using System.Diagnostics;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace Day02
{
    struct PasswordData
    {
        public PasswordData((ushort, ushort) accepted_range, char required_character, string password)
        {
            AcceptedRange = accepted_range;
            RequiredCharacter = required_character;
            Password = password;
        }

        public (ushort, ushort) AcceptedRange { get; }
        public char RequiredCharacter { get; }
        public string Password { get; }
    }

    class Program
    {
        private static PasswordData ParsePasswordLine(string password_line)
        {
            Debug.Assert(!String.IsNullOrEmpty(password_line));

            // Example line: "11-12 c: cccccccccqcccckcc"
            string[] line_components = password_line.Split(' ');
            var required_range = line_components[0];
            var required_character = line_components[1].TrimEnd(':');
            var password = line_components[2];

            // Process required range and stuff it in a tuple
            Debug.Assert(!String.IsNullOrEmpty(required_range));
            string[] range_components = required_range.Split('-');
            if (2 != range_components.Length) { throw new InvalidDataException("range_components length invalid: " + range_components.Length.ToString()); }

            (ushort, ushort) accepted_range = (Convert.ToUInt16(range_components[0]), Convert.ToUInt16(range_components[1]));
            if (accepted_range.Item1 > accepted_range.Item2) { throw new ArgumentException("Range is in an invalid order: " + accepted_range.Item1 + ", " + accepted_range.Item2); }

            // Check required character and password fields, then put everything in a PasswordData and return
            if (1 != required_character.Length) { throw new InvalidDataException("required_character length invalid: " + required_character.Length); }
            if (String.IsNullOrEmpty(password)) { throw new InvalidDataException("password is null or empty"); }

            return new PasswordData(accepted_range, required_character[0], password);
        }

        private static List<PasswordData> ReadData(string data_file_path)
        {
            Debug.Assert(!String.IsNullOrEmpty(data_file_path));

            if (!File.Exists(data_file_path))
            {
                throw new ArgumentException("File " + data_file_path + " does not exist.");
            }

            Console.WriteLine("Reading data...");
            var data = new List<PasswordData>();
            var stopwatch = new Stopwatch();
            stopwatch.Start();
            using (var data_file = new System.IO.StreamReader(data_file_path))
            {
                string? line = null;
                do
                {
                    line = data_file.ReadLine();
                    if (null != line) { data.Add(ParsePasswordLine(line)); }
                } while (line != null);
            }
            stopwatch.Stop();
            Console.WriteLine("Read " + data.Count.ToString() + " lines in " + stopwatch.ElapsedTicks.ToString() + " ticks");

            return data;
        }

        private static uint SolveA(List<PasswordData> password_datas)
        {
            Debug.Assert(0 != password_datas.Count);

            Console.WriteLine("Searching for Solution A...");

            uint valid_count = 0;
            var stopwatch = new Stopwatch();
            stopwatch.Start();
            foreach (var password_data in password_datas)
            {
                Debug.Assert(!String.IsNullOrEmpty(password_data.Password));

                var character_occur_count = password_data.Password.Count(c => c == password_data.RequiredCharacter);
                if (password_data.AcceptedRange.Item1 <= character_occur_count 
                    && password_data.AcceptedRange.Item2 >= character_occur_count)
                {
                    valid_count += 1;
                }
            }
            stopwatch.Stop();
            Console.WriteLine("Found solution in " + stopwatch.ElapsedTicks.ToString() + " ticks");

            return valid_count;
        }

        private static uint SolveB(List<PasswordData> password_datas)
        {
            Debug.Assert(0 != password_datas.Count);

            Console.WriteLine("Searching for Solution B...");

            uint valid_count = 0;
            var stopwatch = new Stopwatch();
            stopwatch.Start();
            foreach (var password_data in password_datas)
            {
                Debug.Assert(!String.IsNullOrEmpty(password_data.Password));

                var password = password_data.Password;
                var character = password_data.RequiredCharacter;
                (int, int) one_indexed_positions = (password_data.AcceptedRange.Item1 - 1, password_data.AcceptedRange.Item2 - 1);

                // Avoid going out of bounds because of switch to one-based indexing
                if (one_indexed_positions.Item1 < 0) { continue; }

                // Only one position may contain the required character.
                if (password[one_indexed_positions.Item1] == character || password[one_indexed_positions.Item2] == character)
                {
                    if (password[one_indexed_positions.Item1] == character && password[one_indexed_positions.Item2] == character)
                    {
                        // Invalid, both indexes can't contain the value, move to the next password
                        continue;
                    }

                } else {
                    // No match, move onto the next password
                    continue;
                }

                // Made it this far in the loop? We have a match!
                valid_count += 1;
            }
            stopwatch.Stop();
            Console.WriteLine("Found solution in " + stopwatch.ElapsedTicks.ToString() + " ticks");

            return valid_count;
        }

        static void Main(string[] command_args)
        {
            if (1 != command_args.Length)
            {
                Console.WriteLine("Usage: dotnet run -- <full_path_to_data_file>");
                return;
            }

            Console.WriteLine("");
            Console.WriteLine("+--------------------------------------------+");
            Console.WriteLine("|  A D V E N T  O F  C O D E     D A Y  0 2  |");
            Console.WriteLine("+--------------------------------------------+");
            Console.WriteLine("");
            Console.WriteLine("Solution A: Find out how many passwords are valid according to policy");
            Console.WriteLine("Solution B: Policy has changed, no longer dealing with a range but abolute positions");
            Console.WriteLine("");

            var data_file_path = command_args[0];
            try
            {
                List<PasswordData> data = ReadData(data_file_path);

                uint solution_a = SolveA(data);
                Console.WriteLine("========================================");
                Console.WriteLine("Solution A: " + solution_a.ToString());
                Console.WriteLine("========================================");

                uint solution_b = SolveB(data);
                Console.WriteLine("========================================");
                Console.WriteLine("Solution B: " + solution_b.ToString());
                Console.WriteLine("========================================");

            }
            catch (Exception exception)
            {
                Console.WriteLine("Exception: " + exception.Message);
            }

        }
    }
}
