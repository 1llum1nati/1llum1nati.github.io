using System;
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

    private class Chrome : MainWindow
    {
        string login, password;
        static readonly string imagesWrap = "page_post_thumb_wrap", 
                               videoWrap = "page_post_thumb_video",
                               audioRow = "audio_row",
                               GIFWrap = "page_post_thumb_unsized",
                               writePath = @"/home/r3pl1c4nt/Docs/a.txt";

        //FileStream aFile = new FileStream(writePath, FileMode.OpenOrCreate);
        StreamWriter sw = new StreamWriter(writePath, false);
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
                     PollsLinks = new List<string>();

        protected internal void Init(string log, string pass)
        {
            login = log;
            password = pass;
            ClearEntrys();
            driver.Navigate().GoToUrl("https://vk.com");
            Login();

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

                for (int i = oldCount; i < Elements.Count; ++i)
                {
                    //sw.WriteLine(Elements[i].Text);
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
                    sw.WriteLine("+++++end+++++");
                }
            }
        }

        protected void ClearEntrys()
        {
            entry1.Text = string.Empty;
            entry2.Text = string.Empty;
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
            }
            else
            {
                TextElements.Add("NO TEXT");
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

                if (TempListGIFs.Length != 0)
                {
                    GIFsLinks.Add(TempListGIFs);
                }
                else
                    GIFsLinks.Add("NO GIFS");

                if (TempListImages.Length != 0)
                {
                    ImagesLinks.Add(TempListImages);
                }
                else
                    ImagesLinks.Add("NO IMAGES");
                if (TempListVideos.Length != 0)
                {
                    VideosLinks.Add(TempListVideos);
                }
                else
                    VideosLinks.Add("NO VIDEOS");
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
                              Polls = new List<IWebElement>();
                              
            Thumbs = Elements[item].FindElements(By.ClassName("page_post_sized_thumbs")).Distinct().ToList();
            Audios = Elements[item].FindElements(By.ClassName("wall_audio_rows")).Distinct().ToList();
            Documents = Elements[item].FindElements(By.ClassName("media_desc__doc")).Distinct().ToList();
            Articles = Elements[item].FindElements(By.ClassName("article_snippet")).Distinct().ToList();
            Polls = Elements[item].FindElements(By.ClassName("media_voting")).Distinct().ToList();

            if (Thumbs.Count > 0)
            {
                SplitThumbsURL(Thumbs);
            }
            else
            {
                ImagesLinks.Add("NO IMAGES");
                VideosLinks.Add("NO VIDEOS");
                GIFsLinks.Add("NO GIFS");
            }

            if (Audios.Count > 0)
            {
                FindAudiosURL(Audios);
            }
            else
            {
                AudiosLinks.Add("NO AUDIOS");
            }

            if (Documents.Count > 0)
            {
                FindDocumentsURL(Documents);
            }
            else
            {
                DocumentsLinks.Add("NO DOCUMENTS");
            }

            if (Articles.Count > 0)
            {
                ArticlesLinks.Add(Articles[0].GetAttribute("href") + "\n");
            }
            else
            {
                ArticlesLinks.Add("NO ARTICLES");
            }

            if (Polls.Count > 0)
            {
                PollsLinks.Add("https://vk.com/poll" + Polls[0].GetAttribute("data-owner-id") + "_" + Polls[0].GetAttribute("data-id") + "\n");
            }
            else
            {
                PollsLinks.Add("NO POLLS");
            }
        }
    }

    protected void OnButton2Clicked(object sender, EventArgs e)
    {
        Chrome Main = new Chrome();
        Main.Init(entry1.Text, entry2.Text);
    }
}
