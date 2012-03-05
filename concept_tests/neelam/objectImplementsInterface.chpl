class Person {
	var firstName : string;
	var lastName : string;

	proc ==(other: Person):bool {
		return this.firstName == other.firstName 
			&& this.lastName == other.lastName;
	}

	proc <(other: Person):bool {
		return this.firstName < other.firstName ||
			(this.firstName == other.firstName
			&& this.lastName < other.lastName);
	}
}

Person implements EqualityComparable;
Person implements LessThanComparable;
