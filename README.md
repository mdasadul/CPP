import com.rabbitmq.client.*;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.PrintWriter;
import java.util.ArrayList;

public class Client {

	private static final String TASK_QUEUE_NAME = "task_queue";

	private static ArrayList<String> listFilesForFolder(final File folder) {
		ArrayList<String> filesName = new ArrayList<String>();
		for (final File fileEntry : folder.listFiles()) {
			if (fileEntry.isDirectory()) {
				listFilesForFolder(fileEntry);
			} else {
				// System.out.println(fileEntry.getName());
				filesName.add(fileEntry.getName());
			}
		}
		return filesName;
	}

	public static void main(String[] argv) throws Exception {
		ConnectionFactory factory = new ConnectionFactory();
		factory.setHost("localhost");
		Connection connection = factory.newConnection();
		Channel channel = connection.createChannel();

		channel.queueDeclare(TASK_QUEUE_NAME, true, false, false, null);

		String path = "/home/asadul/Downloads/tmp/201106"; // Location of the
															// folder related to
															// home
		final File folder = new File(path);
		ArrayList<String> filesName = new ArrayList<String>();
		filesName = listFilesForFolder(folder);
		int numberOfFiles =1;
		System.out.println("Total Number of files: " + filesName.size());
		for (String fileName : filesName) {
			System.out.println(numberOfFiles + " " + fileName);
			String FileNameWithPath = path + "/" + fileName;
			numberOfFiles++;
			channel.basicPublish("", TASK_QUEUE_NAME, MessageProperties.PERSISTENT_TEXT_PLAIN,
					FileNameWithPath.getBytes("UTF-8"));
			System.out.println(" [x] Sent '" + FileNameWithPath + "'");
		}
		channel.close();
		connection.close();
	}
}


import com.rabbitmq.client.*;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.PrintWriter;
import java.util.ArrayList;

import org.apache.poi.hslf.exceptions.EncryptedPowerPointFileException;
import org.apache.tika.detect.DefaultDetector;
import org.apache.tika.exception.TikaException;
import org.apache.tika.metadata.Metadata;
import org.apache.tika.parser.AutoDetectParser;
import org.apache.tika.parser.ParseContext;
import org.apache.tika.parser.Parser;
import org.apache.tika.sax.BodyContentHandler;
import org.xml.sax.ContentHandler;
import org.xml.sax.SAXException;

import java.io.IOException;

public class Worker {

	private static final String TASK_QUEUE_NAME = "task_queue";

	public static void main(String[] argv) throws Exception {
		ConnectionFactory factory = new ConnectionFactory();
		factory.setHost("localhost");
		final Connection connection = factory.newConnection();
		final Channel channel = connection.createChannel();

		channel.queueDeclare(TASK_QUEUE_NAME, true, false, false, null);
		System.out.println(" [*] Waiting for messages. To exit press CTRL+C");

		channel.basicQos(1);

		final Consumer consumer = new DefaultConsumer(channel) {
			@Override
			public void handleDelivery(String consumerTag, Envelope envelope, AMQP.BasicProperties properties,
					byte[] body) throws IOException {
				String message = new String(body, "UTF-8");

				System.out.println(" [x] Received '" + message + "'");
				try {
					doWork(message);
				} finally {
					System.out.println(" [x] Done");
					channel.basicAck(envelope.getDeliveryTag(), false);
				}
			}
		};
		channel.basicConsume(TASK_QUEUE_NAME, false, consumer);
	}

	private static void doWork(String FileNameWithPath) {
		InputStream is = null;
		try {
			
			is = new BufferedInputStream(new FileInputStream(new File(FileNameWithPath)));
			Parser parser = new AutoDetectParser(new DefaultDetector());
			ContentHandler handler = new BodyContentHandler(-1); // unlimited
																	// write
																	// buffer

			Metadata metadata = new Metadata();

			parser.parse(is, handler, metadata, new ParseContext());

			String fileContent = handler.toString().trim();
			fileContent = fileContent.replaceAll("<!--[\\S\\s]*?-->", " ").trim();
			fileContent = fileContent.replaceAll("<!\\[CDATA\\[([\\S\\s]*?)]]>>", "$1").trim();
			fileContent = fileContent.replaceAll("<![?!]\\w+[^>]*>", " ").trim();
			fileContent = fileContent.replaceAll("<(?i)(script|style)[^>]*>[\\S\\s]*?<\\/\\1>", " ").trim();
			fileContent = fileContent.replaceAll("<[^>]*>", " ").trim();
			fileContent = fileContent.replaceAll("&(#(?i)[0-9]+|[a-z0-9]+);", " ").trim();

			fileContent = fileContent.replaceAll("\\s[^\\w&]\\s|[-]{2,}", " ").trim();
			fileContent = fileContent.replaceAll("^\\W+|\\W+$", " ").trim();
			String[] texts = fileContent.split("\\s+");
			String fileName = FileNameWithPath.substring(FileNameWithPath.lastIndexOf('/'));
			// write the metadata and word count to a file
			PrintWriter writer = new PrintWriter(fileName, "UTF-8");
			writer.println("Word Count: " + texts.length);

			for (String name : metadata.names()) {
				String value = metadata.get(name);
				if (value != null) {
					writer.println(name + " : " + value);
				}
			}
			writer.close();

		} catch (IOException e) {
			e.printStackTrace();
		} catch (TikaException e) {
			// e.printStackTrace();
			System.out.println("Encrypted File encountered");
		} catch (SAXException e) {
			e.printStackTrace();
		} catch (EncryptedPowerPointFileException e) {
			e.printStackTrace();
		} catch (NumberFormatException e) {
			System.out.println("NumberFormatException: " + e.getMessage());
		} finally {
			if (is != null) {
				try {
					is.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
	}
}




