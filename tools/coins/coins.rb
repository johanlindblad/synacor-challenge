colors = {
	2 => "red coin",
	3 => "corroded coin",
	5 => "shiny coin",
	7 => "concave coin",
	9 => "blue coin"
}

desired_sum = 399

[7,2,3,5,9].permutation(5).each do |permutation|
	value = permutation[0] + (permutation[1] * (permutation[2]**2)) + (permutation[3]**3) - permutation[4]
	if value == desired_sum
		permutation.each do |n|
			puts "use #{colors[n]}"
		end
		break
	end
end