using System;
using System.IO;
using System.Linq;

class Program
{
    static void Main()
    {
        string execDir = AppContext.BaseDirectory;

        string projectRoot = Directory.GetParent(execDir)
                                      ?.Parent?.Parent?.Parent
                                      ?.FullName ?? execDir;

        string filePath = Path.Combine(projectRoot, "dataTest.txt");

        if (!File.Exists(filePath))
        {
            Console.WriteLine("File not found: " + filePath);
            return;
        }

        string[] lines = File.ReadAllLines(filePath);

        long cksum = 0;
        foreach (string line in lines)
        {
            int[] original = line.Select(c => c - '0').ToArray();
            int[] sortedDesc = original.OrderByDescending(x => x).ToArray();

            Console.WriteLine($"Original: {line}");
            Console.WriteLine($"Sorted desc: {string.Join("", sortedDesc)}");
            cksum += BuildChecksumDigits2(original, sortedDesc);

            Console.WriteLine($"Checksum: {cksum}");
        }
    }

    static int BuildChecksumDigits(int[] original, int[] sortedDesc)
    {
        // Iterate over sorted numbers from largest to smallest
        for (int s = 0; s < sortedDesc.Length; s++)
        {
            int candidate = sortedDesc[s];

            // Find the position of candidate in original
            int pos = Array.IndexOf(original, candidate);

            if (pos == -1)
                continue;

            // If candidate is last → skip to next in sorted list
            if (pos == original.Length - 1)
            {
                // pick the next digit in sorted array *if it exists*
                if (s + 1 < sortedDesc.Length)
                    return int.Parse($"{sortedDesc[s + 1]}{candidate}");

                // if there IS no next digit → no result possible
                return -1;
            }


            // Look at numbers to the right
            int[] rightSegment = original[(pos + 1)..];

            // Find the largest number <= candidate
            int bestRight = rightSegment
                .Where(x => x <= candidate)
                .DefaultIfEmpty(-1)
                .Max();

            // Must find at least something
            if (bestRight >= 0)
            {
                Console.WriteLine($"Checksum digits: {candidate}{bestRight}");
                return int.Parse($"{candidate}{bestRight}");
            }
        }

        return -1; // no combination found
    }

    static long BuildChecksumDigits2(int[] original, int[] sortedDesc)
    {
        int maxDigits = 12;
        var positions = new System.Collections.Generic.List<int>();
        int searchStart = 0;

        int count = Math.Min(maxDigits, sortedDesc.Length);

        for (int i = 0; i < count; i++)
        {
            int digit = sortedDesc[i];

            // Try to find first occurrence starting from searchStart
            int pos = Array.IndexOf(original, digit, searchStart);

            // If not found, wrap around and search from the beginning
            if (pos == -1 && searchStart > 0)
            {
                pos = Array.IndexOf(original, digit, 0);
            }

            if (pos >= 0)
            {
                positions.Add(pos);
                searchStart = pos + 1;

                // Wrap searchStart if it exceeds the array length
                if (searchStart >= original.Length)
                    searchStart = 0;
            }
        }

        if (positions.Count == 0)
            return -1;
        positions.Sort();
        // Build number from digits at recorded positions
        long result = 0;
        foreach (int pos in positions)
        {
            result = result * 10 + original[pos];
        }
        Console.WriteLine($"Result: {result}");
        return result;
    }


}
