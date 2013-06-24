package dozer.mapred;

import java.io.IOException;
import java.util.logging.Logger;

import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.CommandLineParser;
import org.apache.commons.cli.GnuParser;
import org.apache.commons.cli.HelpFormatter;
import org.apache.commons.cli.Option;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.ParseException;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.hbase.HBaseConfiguration;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.TextInputFormat;
import org.apache.hadoop.mapreduce.lib.output.TextOutputFormat;
import org.apache.hadoop.util.GenericOptionsParser;

public class ParseBa {

	public static final String FIELD_START_BEER_NAME = "beer/name: ";

	public static class ParseBaMapper extends
			Mapper<LongWritable, Text, Text, LongWritable> {

		private final Text outKey = new Text();
		private final LongWritable outVal = new LongWritable(1);

		@Override
		protected void map(LongWritable key, Text value, Context context)
				throws IOException, InterruptedException {

			final String s = value.toString();

			try {

				final int beerNameStartIdx = s.indexOf(FIELD_START_BEER_NAME)
						+ FIELD_START_BEER_NAME.length();
				final int beerNameEndIdx = s.indexOf("\n", beerNameStartIdx);
				final String beerName = s.substring(beerNameStartIdx,
						beerNameEndIdx);
				outKey.set(beerName);
				context.write(outKey, outVal);

			} catch (Exception e) {
				Logger.getGlobal().warning("Exception with input:\n" + s);
				Logger.getGlobal().warning(e.getMessage());
			}
		}
	}

	public static class ParseBaReducer extends
			Reducer<Text, LongWritable, Text, LongWritable> {

		private LongWritable outVal = new LongWritable();

		@Override
		protected void reduce(Text key, Iterable<LongWritable> values,
				Context ctx) throws IOException, InterruptedException {

			long sum = 0;
			for (LongWritable v : values) {
				sum += v.get();
			}

			outVal.set(sum);
			ctx.write(key, outVal);
		}

	}

	public static void main(String[] args) throws Exception {

		Configuration conf = HBaseConfiguration.create();
		String[] otherArgs = new GenericOptionsParser(conf, args)
				.getRemainingArgs();

		final Options options = new Options();
		{
			Option o = new Option("i", "input", true, "Input file");
			o.setRequired(true);
			options.addOption(o);
		}
		{
			Option o = new Option("o", "output", true, "Output file");
			o.setRequired(true);
			options.addOption(o);
		}

		final CommandLineParser parser = new GnuParser();
		CommandLine cl = null;
		try {
			cl = parser.parse(options, otherArgs, true);
		} catch (final ParseException e) {
			final HelpFormatter help = new HelpFormatter();
			help.printHelp(Checkpoint.class.getSimpleName(), options);
			System.exit(1);
		}

		Job job = new Job(conf, ParseBa.class.getName());
		job.getConfiguration().set("textinputformat.record.delimiter", "\n\n");
		job.setJarByClass(ParseBa.class);
		job.setNumReduceTasks(2);

		job.setMapperClass(ParseBaMapper.class);
		job.setMapOutputKeyClass(Text.class);
		job.setMapOutputValueClass(LongWritable.class);
		
		job.setCombinerClass(ParseBaReducer.class);
		job.setReducerClass(ParseBaReducer.class);
		job.setOutputKeyClass(Text.class);
		job.setOutputValueClass(LongWritable.class);

		Path inputPath = new Path(cl.getOptionValue("i"));
		Path outputPath = new Path(cl.getOptionValue("o"));
		
		job.setInputFormatClass(TextInputFormat.class);
		TextInputFormat.setInputPaths(job, inputPath);

		job.setOutputFormatClass(TextOutputFormat.class);
		TextOutputFormat.setOutputPath(job, outputPath);

		job.waitForCompletion(true);
	}
}
