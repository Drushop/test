var gulp = require('gulp'), 
    browserSync = require('browser-sync').create(),
    autoprefixer = require('gulp-autoprefixer'),
    sass = require('gulp-sass');

gulp.task('sass', function(done) {
    gulp.src("./sass/*.scss")
        .pipe(sass.sync().on('error', sass.logError))
        .pipe(sass())
        .pipe(autoprefixer(['< 1%', 'ie 8', 'ie 7'], { cascade: true }))
        .pipe(gulp.dest("./css"))
        .pipe(browserSync.stream());


    done();
});

gulp.task('serve', function(done) {

    browserSync.init({
        proxy: "tkdtrade"
    });

    gulp.watch("./sass/*.scss", gulp.series('sass'));
    gulp.watch("./*.tpl.php").on('change', () => {
      browserSync.reload();
      done();
    });

    done();
});

gulp.task('default', gulp.series('sass', 'serve'));