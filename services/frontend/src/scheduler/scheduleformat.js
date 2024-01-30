export function formatSelectedCourses(selectedCourses) {
    let groupedCourses = {};
    for (const course of selectedCourses) {
        if (groupedCourses[course.name] != null) {
            groupedCourses[course.name].push(course);
        }
        else {
            groupedCourses[course.name] = [course];
        }
    }
    return groupedCourses;
}
