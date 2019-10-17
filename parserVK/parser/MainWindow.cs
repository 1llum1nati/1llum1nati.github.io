﻿using System;
using Gtk;
using System.Linq;
using System.Collections.Generic;
using OpenQA.Selenium.Chrome;
using OpenQA.Selenium;
using System.IO;
using Newtonsoft.Json;


public partial class MainWindow : Gtk.Window
{
    public MainWindow() : base(Gtk.WindowType.Toplevel) => Build();

    protected void OnDeleteEvent(object sender, DeleteEventArgs a)
    {
        Application.Quit();
        a.RetVal = true;
    }

    public class Text
    {
        public string ID { get; set; } 
        public string PostText { get; set; }
    }

    public class Image
    {
        public string ID { get; set; }
        public string Img { get; set; }
    }

    public class All
    {
        public string ID { get; set; }
        public string Video { get; set; }
        public string Audio { get; set; }
        public string GIF { get; set; }
        public string Doc { get; set; }
        public string Article { get; set; }
        public string Poll { get; set; }
        public string ThumbedLink { get; set; }
        public string Geotag { get; set; }
        public string Poster { get; set; }
        public string MediaThumbedLink { get; set; }
    }

    private class Chrome : MainWindow
    {
        string login, password;
        static readonly string imagesWrap = "page_post_thumb_wrap",
                               videoWrap = "page_post_thumb_video",
                               audioRow = "audio_row",
                               GIFWrap = "page_post_thumb_unsized",
                               writePath = @"/home/r3pl1c4nt/Docs/a.txt",
                               textPath = @"/home/r3pl1c4nt/Docs/idText.json",
                               imgPath = @"/home/r3pl1c4nt/Docs/idImg.json",
                               allPath = @"/home/r3pl1c4nt/Docs/all.json";

        //FileStream aFile = new FileStream(writePath, FileMode.Create, FileAccess.ReadWrite);
        FileStream idTextFile = new FileStream(textPath, FileMode.Create, FileAccess.ReadWrite);
        FileStream idImgFile = new FileStream(imgPath, FileMode.Create, FileAccess.ReadWrite);
        FileStream allFile = new FileStream(allPath, FileMode.Create, FileAccess.ReadWrite);
        static ChromeDriver driver = new ChromeDriver(InitOptions());
        static IJavaScriptExecutor jsExecutor = driver;

        List<IWebElement> Elements = new List<IWebElement>();
        List<string> PostID = new List<string>(),
                     TextElements = new List<string>(),
                     ImagesLinks = new List<string>(),
                     VideosLinks = new List<string>(),
                     AudiosLinks = new List<string>(),
                     GIFsLinks = new List<string>(),
                     DocumentsLinks = new List<string>(),
                     ArticlesLinks = new List<string>(),
                     ThumbedLinks = new List<string>(),
                     PollsLinks = new List<string>(),
                     Geotags = new List<string>(),
                     PostersLinks = new List<string>(),
                     MediaThumbedLinks = new List<string>();

        protected internal void Init(string log, string pass)
        {
            login = log;
            password = pass;
            ClearEntrys();
            driver.Navigate().GoToUrl("https://vk.com");
            Login();
            //aFile.Close();
            CloseStreams();
            while (true)
            {
                int oldCount = Elements.Count;
                jsExecutor.ExecuteScript("window.scrollTo(0, document.body.scrollHeight);");
                Elements = driver.FindElementsByCssSelector(".wall_post_cont").Distinct().ToList();
                for (var item = oldCount; item != Elements.Count; ++item)
                {
                    PostID.Add("https://vk.com/wall" + Elements[item].GetAttribute("id").Substring(3, Elements[item].GetAttribute("id").Length - 3));
                    FindText(item);
                    FindThumbs(item);
                }


                //sw.WriteLine(Elements[i].Text);
                /*using (StreamWriter sw = new StreamWriter(writePath, true))
                {
                    for (int i = oldCount; i < Elements.Count; ++i)
                    {
                        sw.WriteLine("-----id-----");
                        sw.WriteLine(PostID[i]);
                        sw.WriteLine("-----text-----");
                        sw.WriteLine(TextElements[i]);
                        sw.WriteLine("-----images-----");
                        sw.WriteLine(ImagesLinks[i]);
                        sw.WriteLine("-----videos-----");
                        sw.WriteLine(VideosLinks[i]);
                        sw.WriteLine("-----audios-----");
                        sw.WriteLine(AudiosLinks[i]);
                        sw.WriteLine("-----GIFs-----");
                        sw.WriteLine(GIFsLinks[i]);
                        sw.WriteLine("-----Documents-----");
                        sw.WriteLine(DocumentsLinks[i]);
                        sw.WriteLine("-----Articles-----");
                        sw.WriteLine(ArticlesLinks[i]);
                        sw.WriteLine("-----Polls-----");
                        sw.WriteLine(PollsLinks[i]);
                        sw.WriteLine("-----Thumbed links-----");
                        sw.WriteLine(ThumbedLinks[i]);
                        sw.WriteLine("-----Geotags-----");
                        sw.WriteLine(Geotags[i]);
                        sw.WriteLine("-----Posters-----");
                        sw.WriteLine(PostersLinks[i]);
                        sw.WriteLine("-----Media_link__media-----");
                        sw.WriteLine(MediaThumbedLinks[i]);
                        sw.WriteLine("+++++end+++++");
                    }
                }*/

                using (StreamWriter swText = new StreamWriter(textPath, true))
                {
                    for (int i = oldCount; i < Elements.Count; ++i)
                    {
                        Text temp = new Text
                        {
                            ID = PostID[i].Replace("\n", " "),
                            PostText = TextElements[i].Replace("\n", " ")
                        };
                        swText.WriteLine(JsonConvert.SerializeObject(temp, Formatting.Indented));
                    }
                }

                using (StreamWriter swImg = new StreamWriter(imgPath, true))
                {
                    for (int i = oldCount; i < Elements.Count; ++i)
                    {
                        Image temp = new Image
                        {
                            ID = PostID[i].Replace("\n", " "),
                            Img = ImagesLinks[i].Replace("\n", " ")
                        };
                        swImg.WriteLine(JsonConvert.SerializeObject(temp, Formatting.Indented));
                    }
                }
                using (StreamWriter swAll = new StreamWriter(allPath, true))
                {
                    for (int i = oldCount; i < Elements.Count; ++i)
                    {
                        All temp = new All
                        {
                            ID = PostID[i].Replace("\n", " "),
                            Video = VideosLinks[i].Replace("\n", " "),
                            Audio = AudiosLinks[i].Replace("\n", " "),
                            GIF = GIFsLinks[i].Replace("\n", " "),
                            Doc = DocumentsLinks[i].Replace("\n", " "),
                            Article = ArticlesLinks[i].Replace("\n", " "),
                            Poll = PollsLinks[i].Replace("\n", " "),
                            ThumbedLink = ThumbedLinks[i].Replace("\n", " "),
                            Geotag = Geotags[i].Replace("\n", " "),
                            Poster = PostersLinks[i].Replace("\n", " "),
                            MediaThumbedLink = MediaThumbedLinks[i].Replace("\n", " ")
                        };
                        swAll.WriteLine(JsonConvert.SerializeObject(temp, Formatting.Indented));
                    }
                }
            }
        }


        protected void ClearEntrys()
        {
            entry1.Text = string.Empty;
            entry2.Text = string.Empty;
        }
        protected void CloseStreams()
        {
            idTextFile.Close();
            idImgFile.Close();
            allFile.Close();
        }
        protected static ChromeOptions InitOptions()
        {
            ChromeOptions options = new ChromeOptions();
            options.AddArgument("--incognito");
            options.AddArgument("--start-maximized");
            return options;
        }
        protected void Login()
        {
            driver.FindElementById("index_email").SendKeys(login);
            driver.FindElementById("index_pass").SendKeys(password);
            driver.FindElementById("index_login_button").SendKeys(Keys.Enter);
            //driver.Manage().Timeouts().ImplicitWait = TimeSpan.FromSeconds(10);
        }

        protected void FindText(int item)
        {
            if (Elements[item].FindElements(By.ClassName("wall_post_text")).Count > 0)
            {
                string TextElement = "";
                if (Elements[item].FindElements(By.XPath(".//*[@class='wall_post_text']/span")).Count > 0)
                {
                    TextElement += Elements[item].FindElement(By.ClassName("wall_post_text")).Text.
                    Substring(0, Elements[item].FindElement(By.ClassName("wall_post_text")).Text.Length - 19);
                    jsExecutor.ExecuteScript("arguments[0].style.display = 'block';", Elements[item].FindElement(By.ClassName("wall_post_text")).FindElement(By.TagName("span")));
                    TextElement += Elements[item].FindElement(By.XPath(".//*[@class='wall_post_text']/span")).Text + "\n";
                }
                else
                {
                    TextElement += Elements[item].FindElement(By.ClassName("wall_post_text")).Text;
                }
                TextElements.Add(TextElement);
                PostersLinks.Add("");
            }
            else
            {   
                if (Elements[item].FindElements(By.ClassName("poster__wrap")).Count > 0)
                {
                    PostersLinks.Add(Elements[item].FindElement(By.XPath(".//*[@class='poster__wrap']/*[@class='poster__image']")).GetCssValue("background-image").
                    Substring(5, Elements[item].FindElement(By.XPath(".//*[@class='poster__wrap']/*[@class='poster__image']")).GetCssValue("background-image").Length - 7));
                    TextElements.Add(Elements[item].FindElement(By.XPath(".//*[@class='poster__wrap']/*[@class='poster__text']")).Text);
                }
                else
                {
                    PostersLinks.Add("");
                    TextElements.Add("");
                }
            }
        }

        protected void SplitThumbsURL(List<IWebElement> Thumbs)
        {
            string TempListImages = "",
                   TempListVideos = "",
                   TempListGIFs = "";

            foreach (var target in Thumbs)
            {
                foreach (var links in target.FindElements(By.ClassName(imagesWrap)))
                {
                    if (!links.GetAttribute("class").Contains(videoWrap))
                    {
                        TempListImages += links.GetCssValue("background-image").Substring(5, links.GetCssValue("background-image").Length - 7) + "\n";
                    }
                    else
                    {
                        TempListVideos += "https://vk.com/video" + links.GetAttribute("data-video") + "\n";
                    }
                }

                if (target.FindElements(By.ClassName(GIFWrap)).Count > 0)
                {
                    foreach (var dataGIF in target.FindElements(By.ClassName(GIFWrap)))
                    {
                        TempListGIFs += dataGIF.GetAttribute("href") + "\n";
                    }
                }

                ImagesLinks.Add(TempListImages);
                GIFsLinks.Add(TempListGIFs);
                VideosLinks.Add(TempListVideos);
            }
        }

        protected void FindAudiosURL(List<IWebElement> Audios)
        {
            string TempListAudios = "";
            foreach (var dataAudio in Audios[0].FindElements(By.ClassName(audioRow)))
            {
                TempListAudios += "https://vk.com/audio" + dataAudio.GetAttribute("data-full-id") + "\n";
            }
            AudiosLinks.Add(TempListAudios);
        }

        protected void FindDocumentsURL(List<IWebElement> Documents)
        {
            string TempListDocuments = "";
            foreach (var dataDocument in Documents)
            {
                TempListDocuments += dataDocument.FindElement(By.ClassName("page_doc_icon")).GetAttribute("href") + "\n";
            }
            DocumentsLinks.Add(TempListDocuments);
        }

        protected void FindThumbs(int item)
        {
            List<IWebElement> Thumbs = new List<IWebElement>(),
                              Audios = new List<IWebElement>(),
                              Documents = new List<IWebElement>(),
                              Articles = new List<IWebElement>(),
                              Polls = new List<IWebElement>(),
                              TLinks = new List<IWebElement>(),
                              GTags = new List<IWebElement>(),
                              MLinks = new List<IWebElement>();
                              
            Thumbs = Elements[item].FindElements(By.ClassName("page_post_sized_thumbs")).Distinct().ToList();
            Audios = Elements[item].FindElements(By.ClassName("wall_audio_rows")).Distinct().ToList();
            Documents = Elements[item].FindElements(By.ClassName("media_desc__doc")).Distinct().ToList();
            Articles = Elements[item].FindElements(By.ClassName("article_snippet")).Distinct().ToList();
            Polls = Elements[item].FindElements(By.ClassName("media_voting")).Distinct().ToList();
            TLinks = Elements[item].FindElements(By.ClassName("thumbed_link")).Distinct().ToList();
            GTags = Elements[item].FindElements(By.ClassName("page_media_place_label_inline")).Distinct().ToList();
            MLinks = Elements[item].FindElements(By.ClassName("media_link")).Distinct().ToList();

            if (Thumbs.Count > 0)
            {
                SplitThumbsURL(Thumbs);
            }
            else
            {
                ImagesLinks.Add("");
                VideosLinks.Add("");
                GIFsLinks.Add("");
            }

            if (Audios.Count > 0)
            {
                FindAudiosURL(Audios);
            }
            else
            {
                AudiosLinks.Add("");
            }

            if (Documents.Count > 0)
            {
                FindDocumentsURL(Documents);
            }
            else
            {
                DocumentsLinks.Add("");
            }

            if (Articles.Count > 0)
            {
                ArticlesLinks.Add(Articles[0].GetAttribute("href") + "\n");
            }
            else
            {
                ArticlesLinks.Add("");
            }

            if (Polls.Count > 0)
            {
                PollsLinks.Add("https://vk.com/poll" + Polls[0].GetAttribute("data-owner-id") + "_" + Polls[0].GetAttribute("data-id") + "\n");
            }
            else
            {
                PollsLinks.Add("");
            }
            if (TLinks.Count > 0)
            {
                ThumbedLinks.Add(TLinks[0].FindElement(By.ClassName("thumbed_link__thumb")).GetAttribute("href") + "\n");
            }
            else
            {
                ThumbedLinks.Add("");
            }
            if (MLinks.Count > 0)
            {
                MediaThumbedLinks.Add(MLinks[0].FindElement(By.ClassName("media_link__title")).GetAttribute("href") + "\n");
            }
            else
            {
                MediaThumbedLinks.Add("");
            }
            if (GTags.Count > 0)
            {
                Geotags.Add(GTags[0].Text + "\n");
            }
            else
            {
                Geotags.Add("");
            }

        }
    }

    protected void OnButton2Clicked(object sender, EventArgs e)
    {
        Chrome Main = new Chrome();
        Main.Init(entry1.Text, entry2.Text);
    }
}
