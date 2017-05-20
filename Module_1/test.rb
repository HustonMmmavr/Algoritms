require 'prime'

arr1 = Prime.first(1229)
file = File.open("tes.txt", "r")
arr2 = Array.new
file.each do |line|
	arr2.push line.chomp.to_i
  #делаем что-то со строкой
end
c = 1
0.upto(arr2.length - 1) do |i| 
if arr1[i] != arr2[i] 
	c =0 
	break
end
end
p c