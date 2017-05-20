f = File.open("test.txt", "w")

for i in 1..50000
	f.puts(3.to_s + ' ' + i.to_s)
end

for i in 1..50000
	f.puts(2.to_s + ' ' + i.to_s)
end