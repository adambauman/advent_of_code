using System;
using System.Diagnostics;
using System.Collections.Generic;
using System.IO;

#nullable enable

namespace Day01
{
    class Program
    {
        public static List<ushort> ReadData(string data_file_path)
        {
            Debug.Assert(!String.IsNullOrEmpty(data_file_path));

            if (!File.Exists(data_file_path))
            {
                throw new ArgumentException("File " + data_file_path + " does not exist.");
            }

            Console.WriteLine("Reading data...");
            var data = new List<ushort>();
            using (var data_file = new System.IO.StreamReader(data_file_path))
            {
                string? line = null;
                do
                {
                    line = data_file.ReadLine();
                    if (null != line) { data.Add(ushort.Parse(line)); }
                } while (line != null);
            }
            Console.WriteLine("Read " + data.Count.ToString() + " lines");

            return data;
        }

        public static uint SolveA(List<ushort> data)
        {
            Debug.Assert(0 != data.Count);

            ushort? first_number = null;
            ushort? second_number = null;

            var stopwatch = new Stopwatch();
            stopwatch.Start();
            Console.WriteLine("Searching for solution A...");
            foreach (var first_data in data)
            {
                foreach (var second_data in data)
                {
                    if (2020 != first_data + second_data) { continue; }
                    second_number = second_data;
                }

                if (null == second_number) { continue; }

                first_number = first_data;
                break;
            }
            Debug.Assert(null != first_number && null != second_number);

            stopwatch.Stop();
            Console.WriteLine("Found solution in " + stopwatch.ElapsedTicks.ToString() + " ticks");
            Console.WriteLine("First Number: " + first_number.ToString() + " Second Number: " + second_number.ToString());

            return Convert.ToUInt32(first_number * second_number);
        }

        public static uint SolveB(List<ushort> data)
        {
            Debug.Assert(0 != data.Count);

            ushort? first_number = null;
            ushort? second_number = null;
            ushort? third_number = null;

            var stopwatch = new Stopwatch();
            stopwatch.Start();
            Console.WriteLine("Searching for solution B...");
            foreach (var first_data in data)
            {
                foreach (var second_data in data)
                {
                    foreach (var third_data in data)
                    {
                        if (2020 != first_data + second_data + third_data) { continue; }
                        third_number = third_data;
                    }
                    if (null == third_number) { continue; }

                    second_number = second_data;
                    break;
                }

                if (null == second_number && null == third_number) { continue; }

                first_number = first_data;
                break;
            }
            Debug.Assert(null != first_number && null != second_number);

            stopwatch.Stop();
            Console.WriteLine("Found solution in " + stopwatch.ElapsedTicks.ToString() + " ticks");
            Console.WriteLine(
                "First Number: " + first_number.ToString() + 
                " Second Number: " + second_number.ToString() +
                " Third Number: " + third_number.ToString()
                );

            return Convert.ToUInt32(first_number * second_number * third_number);
        }

        static void Main(string[] command_args)
        {
            // Goal is adding to "2020", values in data should not exceed ushort max. ReadData will
            // throw if there is an overrun.
            if (1 != command_args.Length)
            {
                Console.WriteLine("Usage: dotnet run -- <full_path_to_data_file>");
                return;
            }

            var data_file_path = command_args[0];
            try
            {
                List<ushort> data = ReadData(data_file_path);
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
